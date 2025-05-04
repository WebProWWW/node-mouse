
#include <napi.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>

#pragma comment(lib, "winmm.lib")

std::atomic<bool> stopLoopClick(false);

int sizeofInput = sizeof(INPUT);

INPUT inputMouseClick = []() {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
    input.mi.mouseData = 0;
    input.mi.dwExtraInfo = 0;
    input.mi.time = 0;
    return input;
}();

void ClickMouse() { SendInput(1, &inputMouseClick, sizeofInput); }

uint32_t performanceNow() {
    using namespace std::chrono;
    static auto start = steady_clock::now();
    auto now = steady_clock::now();
    return duration_cast<milliseconds>(now - start).count();
}

uint32_t nowTime() {
    uint32_t perfStart = performanceNow();
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    uint32_t totalMs = tm.tm_hour*3600000 + tm.tm_min*60000 + tm.tm_sec*1000 + ms.count();
    uint32_t perfEnd = performanceNow();
    return totalMs + (perfEnd - perfStart) + 2;
}

void LoopClickMouse(Napi::Env &env, uint32_t &startTime, std::vector<uint32_t> &gaps, Napi::Function cbDelay, Napi::Function cbDone) {
    stopLoopClick = false;
    timeBeginPeriod(1);
    cbDelay.Call({ Napi::Number::New(env, startTime - nowTime()) });
    auto tsCbDone = Napi::ThreadSafeFunction::New(env, cbDone, "CallbackDone", 0, 1);
    std::thread([startTime, gaps, tsCbDone]() {
        using namespace std::chrono;
        while (nowTime() < startTime) {
            if (stopLoopClick.load()) {
                timeEndPeriod(1);
                return;
            }
            std::this_thread::sleep_for(milliseconds(1));
        }
        uint32_t diffGD = 0;
        uint32_t diffD = nowTime() - startTime;
        for (uint32_t gap : gaps) {
            uint32_t perfGDStart = performanceNow();
            uint32_t gapD = gap - (diffD + diffGD);
            std::this_thread::sleep_for(milliseconds(gapD));
            ClickMouse();
            diffD = 0;
            diffGD = (performanceNow() - perfGDStart) - gapD;
        }
        timeEndPeriod(1);
        tsCbDone.BlockingCall([](Napi::Env env, Napi::Function cbDone) {
            cbDone.Call(env.Global(), {});
        });
    }).detach();
}

Napi::Value TypeError(Napi::Env &env, std::string msg) {
    Napi::TypeError::New(env, msg).ThrowAsJavaScriptException();
    return env.Null();
}

Napi::Value LoopClick(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 3) { return TypeError(env, "Ожидается (timeMs:Number, gaps:Array, callback:Function)"); }
    if (!info[0].IsNumber()) { TypeError(env, "Первый параметр ожидает тип Number время старта в миллисекундах"); }
    if (!info[1].IsArray()) { TypeError(env, "Второй параметр ожидает тип Array задержки в миллисекундах между кликами."); }
    if (!info[2].IsFunction()) { TypeError(env, "Третий параметр ожидает Callback функцию которая выполняется перед началом кликов."); }
    if (!info[3].IsFunction()) { TypeError(env, "Третий параметр ожидает Callback функцию которая выполняется после завершения кликов."); }
    uint32_t        startTime   = info[0].As<Napi::Number>().Uint32Value();
    Napi::Array     gaps        = info[1].As<Napi::Array>();
    Napi::Function  cbDelay     = info[2].As<Napi::Function>();
    Napi::Function  cbDone      = info[3].As<Napi::Function>();
    std::vector<uint32_t> gapsArr;
    for (uint32_t i = 0; i < gaps.Length(); i++) {
        gapsArr.push_back( gaps.Get(i).As<Napi::Number>().Uint32Value() );
    }
    LoopClickMouse(env, startTime, gapsArr, cbDelay, cbDone);
    return env.Undefined();
}

Napi::Value Click(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    ClickMouse();
    return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("stopLoopClick", Napi::Function::New(env, [](const Napi::CallbackInfo& info) { stopLoopClick = true; }));
    exports.Set("loopClick", Napi::Function::New(env, LoopClick));
    exports.Set("click", Napi::Function::New(env, Click));
    return exports;
}

NODE_API_MODULE(mouse, Init)
