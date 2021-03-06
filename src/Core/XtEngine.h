/**
 * XorTech Source Code
 */
#pragma once

#include <Core/Render/IXtRenderDevice.h>
#include <Core/Input/XtInputManager.h>
#include <Core/IXtPlatform.h>
#include <Core/IXtTime.h>
#include <Core/XtDeviceParams.h>
#include <XtCommon.h>
#include "XtMetadata.h"

namespace xt {
    using namespace input;
    using namespace render;

    class XtEngine {
    private:
        static XtEngine* _instance;

        IXtPlatform*     _platform;
        XtDeviceParams   _initParams;
        XtInputManager*  _inputManager;
        IXtRenderDevice* _renderDevice;

        XtMetadata*     _metadata;

        IXtTime*        _systemTime;
        IXtTime*        _gameTime;

        bool            _inLoop;
        bool            _isActive;
        double          _lastFrameTime;
    public:
        XtEngine();
        ~XtEngine();

        XtEngine(const XtEngine& copy)            = delete;
        XtEngine(const XtEngine&& move)           = delete;
        XtEngine& operator=(const XtEngine& copy) = delete;
        XtEngine& operator=(XtEngine&& move)      = delete;

        static XtEngine* getInstance();
        static void release();

        /**
         * @brief инициализация базовых подсистем движка
         * @param deviceParams - параметры видеоустройства
         * @return результат инициализации
         */
        bool init(const XtDeviceParams& deviceParams = XtDeviceParams());
        /**
         * @brief обновляет все подсистемы движка
         */
		void update();
        /**
         * @brief Возвращает данные о работоспособности движка
         * @return
         */
        bool isOnline() const;
        /**
         * @brief Останавливает основной цикл обработки
         */
        void offline();
        /**
         * @brief Ставит цикл обработки на паузу
         */
        void pause();
        /**
         * @brief Возвращает движок к обработке подсистем
         */
        void resume();

		bool isActive() const { return _isActive; }

        IXtTime* getSystemTime() { return _systemTime; }
        IXtTime* getGameTime() { return _gameTime; }
        IXtPlatform* getCurrentPlatform() { return _platform; }
        XtInputManager* getInputManager() { return _inputManager; }
        IXtRenderDevice* getRenderDevice() { return _renderDevice; }
		double getLastFrameTime() const { return _lastFrameTime; }

		void setLastFrameTime(double time) { _lastFrameTime = time; }
    };
}