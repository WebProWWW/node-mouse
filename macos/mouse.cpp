
#include <napi.h>
#include <ApplicationServices/ApplicationServices.h>

void ClickMouse() {
    CGEventRef event = CGEventCreate(nullptr);
    CGPoint mouseLocation = CGEventGetLocation(event);
    CFRelease(event);
    CGEventRef mouseDown = CGEventCreateMouseEvent(nullptr, kCGEventLeftMouseDown, mouseLocation, kCGMouseButtonLeft);
    CGEventRef mouseUp = CGEventCreateMouseEvent(nullptr, kCGEventLeftMouseUp, mouseLocation, kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, mouseDown);
    CGEventPost(kCGHIDEventTap, mouseUp);
    CFRelease(mouseDown);
    CFRelease(mouseUp);
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
