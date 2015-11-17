Middlewares are special libraries. They may be included in multiple projects
but cannot be build as a static library because they are configured by a
header-file in the specific project where they are used.
Because each project has a potentially different configuration due to different
header files, we can't just link against a static library.
Instead we instruct the buildsystem to include all the codefiles while building
the main project. This means for each project that uses a certain middleware,
the code is compiled again with the potentially different config-header-file.

Examples are FreeRTOS with its FreeRTOSConfig.h file oder the STM32 HAL with
the stm32f4xx_hal_conf.h

I called them middlewares because I lacked a better name, they are not always
middlewares in the sense of the name (the HAL for example).
