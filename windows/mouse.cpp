
#include <napi.h>
#include <Windows.h>

void ClickMouse() {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

Napi::Value Click(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    ClickMouse();
    return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("click", Napi::Function::New(env, Click));
    return exports;
}

NODE_API_MODULE(mouse, Init)
