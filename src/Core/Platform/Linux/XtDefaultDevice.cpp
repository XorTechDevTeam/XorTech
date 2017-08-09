#include "XtDefaultDevice.h"
#include <Core/Render/OpenGL/XtOpenGL.h>
#include <Core/XtEngine.h>

namespace xt {
    namespace platform {
        XtDefaultDevice::~XtDefaultDevice() {
            glfwTerminate();
        }

        bool XtDefaultDevice::createDevice(int width, int height, bool fullscreen, bool reinit) {
            if (!glfwInit()) {
                //TODO: log here
                return false;
            }

            /**
             * OpenGL 3.3
             */
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            _window = glfwCreateWindow(width, height, XT_PROJECT_NAME, fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
            if (!_window)
            {
                glfwTerminate();
                //TODO: log error
                return false;
            }

            glfwMakeContextCurrent(_window);

            if (!reinit) {
                _renderDevice = new render::XtOpenGL(width, height);
            }

            glfwSetKeyCallback(_window, &XtDefaultDevice::glfwOnKeyboardEvent);
            glfwSetMouseButtonCallback(_window, &XtDefaultDevice::glfwOnMouseInput);
            glfwSetWindowSizeCallback(_window, &XtDefaultDevice::glfwOnWindowResize);
            glfwSetScrollCallback(_window, &XtDefaultDevice::glfwOnScroll);

            return true;
        }

        void XtDefaultDevice::onDeviceUpdate(float delta) {
            _renderDevice->drawFrame(delta);
            glfwSwapBuffers(_window);
            glfwPollEvents();
        }

        bool XtDefaultDevice::isDeviceActive() {
            return !static_cast<bool>(glfwWindowShouldClose(_window));
        }


        void XtDefaultDevice::onResize(int width, int height) {
            //TODO: log event here
        }

        void XtDefaultDevice::switchFullscreenMode(bool mode) {
            if (mode && _renderDevice->isFullscreen()) return;

            glfwDestroyWindow(_window);

            int width = 0;
            int height = 0;

            if (mode) {
                GLFWmonitor *pMonitor = glfwGetPrimaryMonitor();
                const GLFWvidmode *vMode = glfwGetVideoMode(pMonitor);

                width = vMode->width;
                height = vMode->height;
                _renderDevice->setFullscreen(mode);
            } else {
                _renderDevice->getScreenResolution(width, height);
                _renderDevice->setFullscreen(mode);
            }

            createDevice(width, height, mode, true);
        }

        /**
         * GLFW Callbacks
         */

        void XtDefaultDevice::glfwOnKeyboardEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {
            const KeyboardEvent keyboardEvent = {
                    key,
                    static_cast<bool>(mods & GLFW_MOD_ALT),
                    static_cast<bool>(mods & GLFW_MOD_SHIFT),
                    static_cast<bool>(mods & GLFW_MOD_SUPER),
                    static_cast<bool>(mods & GLFW_MOD_CONTROL),
                    action == GLFW_PRESS ? KeyboardEvent::PressType::PRESS_DOWN : KeyboardEvent::PressType::PRESS_UP
            };

            xt::XtEngine::getInstance()->getInputManager()->onKeyboardKey(keyboardEvent);
        }

        void XtDefaultDevice::glfwOnMouseInput(GLFWwindow *window, int button, int action, int mods) {
            double x = 0.0;
            double y = 0.0;

            const TapInfo::TapType tapType = action == GLFW_PRESS ? TapInfo::TapType::TAP_DOWN : TapInfo::TapType::TAP_UP;

            glfwGetCursorPos(window, &x, &y);

            const TapInfo tapInfo = { tapType, x, y };
            xt::XtEngine::getInstance()->getInputManager()->onTouch({ tapInfo });
        }

        void XtDefaultDevice::glfwOnScroll(GLFWwindow *window, double xoffset, double yoffset) {
            xt::XtEngine::getInstance()->getInputManager()->onScroll(xoffset, yoffset);
        }

        void XtDefaultDevice::glfwOnWindowResize(GLFWwindow *window, int width, int height) {
            xt::XtEngine::getInstance()->getCurrentDevice()->onResize(width, height);
        }
    }
}