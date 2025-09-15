#include <unordered_map>
#include <string>
#include <iostream>
#include <windows.h> // for GetAsyncKeyState

#define LEFT_MOUSE_BUTTON 0
#define RIGHT_MOUSE_BUTTON 1

class InputSystem {
public:
    enum class KeyState {
        Up,
        Down,
        Pressed,
        Released
    };

    // Call at the beginning of each frame
    void newFrame() {

        for (auto &kv : keyStates) {
            if (kv.second == KeyState::Pressed) kv.second = KeyState::Down;
            else if (kv.second == KeyState::Released) kv.second = KeyState::Up;
        }
        for (auto &kv : mouseStates) {
            if (kv.second == KeyState::Pressed) kv.second = KeyState::Down;
            else if (kv.second == KeyState::Released) kv.second = KeyState::Up;
        }
        pollKeyboard();
        pollMouse();
    }

    // Mapping API
    void bindAction(const std::string &name, int key) {
        keyBindings[name] = key;
    }

    void bindMouseAction(const std::string &name, int button) {
        mouseBindings[name] = button;
    }

    // Query API
    bool isActionDown(const std::string &name) const {
        return check(name, KeyState::Down) || check(name, KeyState::Pressed);
    }

    bool wasActionPressed(const std::string &name) const {
        return check(name, KeyState::Pressed);
    }

    bool wasActionReleased(const std::string &name) const {
        return check(name, KeyState::Released);
    }

    double getMouseX() const { return mouseX; }
    double getMouseY() const { return mouseY; }

private:
    std::unordered_map<int, KeyState> keyStates;
    std::unordered_map<int, KeyState> mouseStates;
    std::unordered_map<std::string, int> keyBindings;
    std::unordered_map<std::string, int> mouseBindings;

    double mouseX = 0.0, mouseY = 0.0;

    bool check(const std::string &name, KeyState st) const {
        auto it = keyBindings.find(name);
        if (it != keyBindings.end()) {
            auto ks = keyStates.find(it->second);
            if (ks != keyStates.end() && ks->second == st) return true;
        }
        auto im = mouseBindings.find(name);
        if (im != mouseBindings.end()) {
            auto ms = mouseStates.find(im->second);
            if (ms != mouseStates.end() && ms->second == st) return true;
        }
        return false;
    }

    // Polling raw input from Windows
    void pollKeyboard() {
        for (auto &kv : keyBindings) {
            int vk = kv.second;
            short state = GetAsyncKeyState(vk);
            bool down = (state & 0x8000) != 0;

            KeyState &st = keyStates[vk];
            if (down) {
                if (st == KeyState::Up || st == KeyState::Released) st = KeyState::Pressed;
            } else {
                if (st == KeyState::Down || st == KeyState::Pressed) st = KeyState::Released;
            }
        }
    }

    void pollMouse() {
        POINT p;
        GetCursorPos(&p);
        mouseX = static_cast<double>(p.x);
        mouseY = static_cast<double>(p.y);

        for (auto &kv : mouseBindings) {
            int btn = kv.second;
            bool down = false;
            if (btn == LEFT_MOUSE_BUTTON) down = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
            else if (btn == RIGHT_MOUSE_BUTTON) down = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;

            KeyState &st = mouseStates[btn];
            if (down) {
                if (st == KeyState::Up || st == KeyState::Released) st = KeyState::Pressed;
            } else {
                if (st == KeyState::Down || st == KeyState::Pressed) st = KeyState::Released;
            }
        }
    }
};
