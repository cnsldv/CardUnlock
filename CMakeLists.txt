cmake_minimum_required(VERSION 2.8)

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_C_COMPILER "arm-vita-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-vita-eabi-g++")

project(CardUnlock)
set(TITLE_ID "CARDUNLOK")
set(TITLE_NAME "CardUnlock")

set(CMAKE_C_FLAGS "-Wl,-q -Wall -O2 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

include_directories(
)

link_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

if (NOT ${RELEASE})
	add_definitions(-DENABLE_LOGGING)
endif()

add_executable(${PROJECT_NAME}.elf
	main.c
	debugScreenFont.c
)

target_link_libraries(${PROJECT_NAME}.elf
	SceDisplay_stub
	SceCtrl_stub
	ScePower_stub
	SceVshBridge_stub
)

add_custom_target(${PROJECT_NAME}.velf ALL
	COMMAND vita-elf-create ${PROJECT_NAME}.elf ${PROJECT_NAME}.velf
)

add_custom_target(eboot.bin ALL
	COMMAND vita-make-fself ${PROJECT_NAME}.velf eboot.bin
)

add_custom_target(${PROJECT_NAME}.vpk ALL
	COMMAND vita-mksfoex -s TITLE_ID=${TITLE_ID} "${TITLE_NAME}" param.sfo
	COMMAND vita-pack-vpk -s param.sfo -b eboot.bin
		-a ${PROJECT_SOURCE_DIR}/sce_sys/icon0.png=sce_sys/icon0.png
		-a ${PROJECT_SOURCE_DIR}/sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png
		-a ${PROJECT_SOURCE_DIR}/sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png
		-a ${PROJECT_SOURCE_DIR}/sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml
		${PROJECT_NAME}.vpk
)

add_custom_target(vpksend
	COMMAND curl -T ${PROJECT_NAME}.vpk ftp://$(PSVITAIP):1337/ux0:/
	DEPENDS ${PROJECT_NAME}.vpk
)

add_custom_target(send
	COMMAND curl -T eboot.bin ftp://$(PSVITAIP):1337/ux0:/app/${TITLE_ID}/
	DEPENDS eboot.bin
)

add_dependencies(${PROJECT_NAME}.velf ${PROJECT_NAME}.elf)
add_dependencies(eboot.bin ${PROJECT_NAME}.velf)
add_dependencies(${PROJECT_NAME}.vpk eboot.bin)
