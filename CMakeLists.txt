set (PROJECT neoPixels)
cmake_minimum_required(VERSION 3.18)
set (PICO_BOARD pico_w)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${PROJECT} C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

pico_sdk_init()

add_executable(${PROJECT} 
		ws2812.cpp 
		Colour.cpp 
		LedStrip.cpp 
		ws2812Strip.cpp 
		AnimationHelper.cpp
		Commander.cpp
		Segment.cpp
		IntervalSegment.cpp
		ColourProvider.hpp
		ColourWheelProvider.cpp
		Tokeniser.cpp
		BaseDefinedObject.hpp
		ObjectParsers.cpp
		ObjectStore.cpp
		ColourSequenceProvider.hpp
		SimpleProvider.hpp
		NumberProvider.hpp
		ColourFadeProvider.cpp
		MappedSegment.hpp
		RandomLedSequence.hpp
		NumberSequenceProvider.hpp
		SimpleSequenceProvider.hpp
		DiscreteSegment.cpp
		ProviderParsers.cpp
		RandomNumberProvider.hpp
		RandomNumberSetProvider.cpp
		ExecutableItem.cpp
		Action.cpp
		ShiftAction.hpp
		SetAction.hpp
		ExecutableItemParsers.cpp
		CycleAction.hpp
		Block.cpp
		SegmentProvider.hpp
		BooleanProvider.hpp
		ExecutableTypeProvider.hpp
		SegmentParsers.cpp
		Scripting.cpp
		Trace.cpp
		SegmentSequenceProvider.hpp
		NumberIntervalProvider.hpp
		ColourHSVProvider.hpp
		NumberSineProvider.hpp
		FadeAction.hpp
		)

target_link_libraries(${PROJECT}
        pico_stdlib              # for core functionality
        hardware_pio
		pico_bootrom
        )
        
pico_enable_stdio_usb(${PROJECT} 1)
pico_enable_stdio_uart(${PROJECT} 0)

pico_generate_pio_header(${PROJECT} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT})
