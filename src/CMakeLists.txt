function(broker_add_compile_definitions scope define)
	message(STATUS "broker_add_compile_definitions ${scope} ${define}")
	target_compile_definitions(mosquitto ${scope} ${define})
	if (WITH_EMBEDD_BROKER)
		target_compile_definitions(libmosquitto_broker ${scope} ${define})
		if (WITH_STATIC_LIBRARIES)
			target_compile_definitions(libmosquitto_broker_static ${scope} ${define})
		endif()
	endif()
endfunction()

function(broker_include_directories scope file)
	message(STATUS "broker_sources ${scope} ${file}")
	target_include_directories(mosquitto ${scope} ${file})
	if (WITH_EMBEDD_BROKER)
		target_include_directories(libmosquitto_broker ${scope} ${file})
		if (WITH_STATIC_LIBRARIES)
			target_include_directories(libmosquitto_broker_static ${scope} ${file})
		endif()
	endif()
endfunction()

function(broker_sources scope file)
	message(STATUS "broker_sources ${scope} ${file}")
	target_sources(mosquitto ${scope} ${file})
	if (WITH_EMBEDD_BROKER)
		target_sources(libmosquitto_broker ${scope} ${file})
		if (WITH_STATIC_LIBRARIES)
			target_sources(libmosquitto_broker_static ${scope} ${file})
		endif()
	endif()
endfunction()

function(broker_link_libraries scope library)
	message(STATUS "broker_link_libraries ${scope} ${library}")
	target_link_libraries(mosquitto ${scope} ${library})
	if (WITH_EMBEDD_BROKER)
		target_link_libraries(libmosquitto_broker ${scope} ${library})
		if (WITH_STATIC_LIBRARIES)
			target_link_libraries(libmosquitto_broker_static  ${scope} ${library})
		endif()
	endif()
endfunction()

set(BROKER_SRC
	../lib/alias_mosq.c ../lib/alias_mosq.h
	bridge.c bridge_topic.c
	conf.c
	conf_includedir.c
	context.c
	control.c
	database.c
	handle_auth.c
	handle_connack.c
	handle_connect.c
	handle_disconnect.c
	../lib/handle_ping.c
	../lib/handle_pubackcomp.c
	handle_publish.c
	../lib/handle_pubrec.c
	../lib/handle_pubrel.c
	../lib/handle_suback.c
	handle_subscribe.c
	../lib/handle_unsuback.c
	handle_unsubscribe.c
	keepalive.c
	lib_load.h
	logging.c
	loop.c
	../lib/memory_mosq.c ../lib/memory_mosq.h
	memory_public.c
	mosquitto.c
	../include/mosquitto_broker.h mosquitto_broker_internal.h
	../lib/misc_mosq.c ../lib/misc_mosq.h
	mux.c mux.h mux_epoll.c mux_poll.c
	net.c
	../lib/net_mosq_ocsp.c ../lib/net_mosq.c ../lib/net_mosq.h
	../lib/packet_datatypes.c
	../lib/packet_mosq.c ../lib/packet_mosq.h
	password_mosq.c password_mosq.h
	persist_read_v234.c persist_read_v5.c persist_read.c
	persist_write_v5.c persist_write.c
	persist.h
	plugin.c plugin_public.c
	property_broker.c
	../lib/property_mosq.c ../lib/property_mosq.h
	read_handle.c
	../lib/read_handle.h
	retain.c
	security.c security_default.c
	../lib/send_mosq.c ../lib/send_mosq.h
	send_auth.c
	send_connack.c
	../lib/send_connect.c
	../lib/send_disconnect.c
	../lib/send_publish.c
	send_suback.c
	../lib/send_subscribe.c
	send_unsuback.c
	../lib/send_unsubscribe.c
	session_expiry.c
	../lib/strings_mosq.c
	subs.c
	sys_tree.c sys_tree.h
	../lib/time_mosq.c
	../lib/tls_mosq.c
	topic_tok.c
	../lib/util_mosq.c ../lib/util_topic.c ../lib/util_mosq.h
	../lib/utf8_mosq.c
	websockets.c
	will_delay.c
	../lib/will_mosq.c ../lib/will_mosq.h)

add_executable(mosquitto ${BROKER_SRC} main.c signals.c)

if (WITH_EMBEDD_BROKER)
	add_library(libmosquitto_broker SHARED ${BROKER_SRC} embedd_broker.c)
	if (WITH_STATIC_LIBRARIES)
		add_library(libmosquitto_broker_static STATIC ${BROKER_SRC} embedd_broker.c)
	endif()
endif()
      
broker_include_directories(PUBLIC "${mosquitto_SOURCE_DIR}")
broker_include_directories(PUBLIC "${mosquitto_SOURCE_DIR}/include")
broker_include_directories(PRIVATE "${mosquitto_SOURCE_DIR}/common")
broker_include_directories(PRIVATE "${mosquitto_SOURCE_DIR}/lib")
broker_include_directories(PRIVATE "${mosquitto_SOURCE_DIR}/libcommon")
broker_include_directories(PRIVATE "${mosquitto_SOURCE_DIR}/src")

if(WITH_BUNDLED_DEPS)
	broker_include_directories(PRIVATE "${mosquitto_SOURCE_DIR}/deps")
endif()

# CHECK_INCLUDE_FILES(sys/event.h HAVE_SYS_EVENT_H)
# if(HAVE_SYS_EVENT_H)
# 	broker_add_compile_definitions(PRIVATE "WITH_KQUEUE")
# endif()

find_path(HAVE_SYS_EPOLL_H sys/epoll.h)
if (HAVE_SYS_EPOLL_H)
	broker_add_compile_definitions(PRIVATE "WITH_EPOLL")
endif()

option(INC_BRIDGE_SUPPORT
	"Include bridge support for connecting to other brokers?" ON)

if (INC_BRIDGE_SUPPORT)
	broker_sources(PRIVATE bridge.c)
	broker_add_compile_definitions(PRIVATE "WITH_BRIDGE")
endif (INC_BRIDGE_SUPPORT)


option(USE_LIBWRAP
	"Include tcp-wrappers support?" OFF)

if (USE_LIBWRAP)
	broker_sources(wrap)
	broker_add_compile_definitions(PRIVATE "WITH_WRAP")
endif (USE_LIBWRAP)

option(INC_DB_UPGRADE
	"Include database upgrade support? (recommended)" ON)

option(INC_MEMTRACK
	"Include memory tracking support?" ON)
if (INC_MEMTRACK)
	broker_add_compile_definitions(PUBLIC "WITH_MEMORY_TRACKING")
endif (INC_MEMTRACK)

option(WITH_PERSISTENCE
	"Include persistence support?" ON)
if (WITH_PERSISTENCE)
	broker_add_compile_definitions(PRIVATE "WITH_PERSISTENCE")
endif (WITH_PERSISTENCE)

option(WITH_SYS_TREE
	"Include $SYS tree support?" ON)
if (WITH_SYS_TREE)
	broker_add_compile_definitions(PRIVATE "WITH_SYS_TREE")
endif (WITH_SYS_TREE)

option(WITH_ADNS
	"Include ADNS support?" OFF)

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
	option(WITH_SYSTEMD
		"Include systemd support?" OFF)
	if (WITH_SYSTEMD)
		broker_add_compile_definitions(PRIVATE "WITH_SYSTEMD")
		find_library(SYSTEMD_LIBRARY systemd)
		broker_link_libraries(PRIVATE "${SYSTEMD_LIBRARY}")
	endif (WITH_SYSTEMD)
endif (CMAKE_SYSTEM_NAME STREQUAL Linux)

option(WITH_WEBSOCKETS "Include websockets support?" OFF)
option(STATIC_WEBSOCKETS "Use the static libwebsockets library?" OFF)
if (WITH_WEBSOCKETS)
	find_package(libwebsockets)
	broker_add_compile_definitions(PRIVATE "WITH_WEBSOCKETS")
endif (WITH_WEBSOCKETS)

option(WITH_CONTROL "Include $CONTROL topic support?" ON)
if (WITH_CONTROL)
  	broker_add_compile_definitions(PRIVATE "WITH_CONTROL")
endif (WITH_CONTROL)

if (WIN32 OR CYGWIN)
	broker_sources(PRIVATE service.c)
endif (WIN32 OR CYGWIN)

broker_add_compile_definitions(PRIVATE "WITH_BROKER")

if (WITH_DLT)
    message(STATUS "DLT_LIBDIR = ${DLT_LIBDIR}")
    target_link_directories(mosquitto PRIVATE ${DLT_LIBDIR})
    broker_link_libraries(PRIVATE "${DLT_LIBRARIES}")
    broker_add_compile_definitions(PRIVATE "WITH_DLT")
endif (WITH_DLT)

if (WITH_TLS)
        broker_link_libraries(PRIVATE OpenSSL::SSL)
endif (WITH_TLS)

# Check for getaddrinfo_a
include(CheckLibraryExists)
check_library_exists(anl getaddrinfo_a  "" HAVE_GETADDRINFO_A)
if (HAVE_GETADDRINFO_A AND WITH_ADNS)
	broker_add_compile_definitions(PRIVATE "WITH_ADNS")
	broker_add_compile_definitions(PRIVATE "HAVE_GETADDRINFO_A")
	broker_link_libraries(PRIVATE anl)
endif (HAVE_GETADDRINFO_A AND WITH_ADNS)


if (UNIX)
	if (APPLE)
		broker_link_libraries(PRIVATE dl)
		broker_link_libraries(PRIVATE m)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "OpenBSD")
		broker_link_libraries(PRIVATE m)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "NetBSD")
		broker_link_libraries(PRIVATE m)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "Haiku")
		broker_link_libraries(PRIVATE network)
		broker_link_libraries(PRIVATE m)
	elseif(QNX)
		broker_link_libraries(PRIVATE socket)
		broker_link_libraries(PRIVATE m)
	else(APPLE)
		broker_link_libraries(PRIVATE dl)
		broker_link_libraries(PRIVATE m)
		find_library(LIBRT rt)
		if (LIBRT)
			broker_link_libraries(PRIVATE rt)
		endif(LIBRT)
	endif (APPLE)
endif (UNIX)

if (WIN32)
	broker_link_libraries(PRIVATE ws2_32)
endif (WIN32)

if (WITH_WEBSOCKETS)
	if (STATIC_WEBSOCKETS)
		broker_link_libraries(PRIVATE websockets)
		if (WIN32)
			broker_link_libraries(PRIVATE iphlpapi)
			target_link_directories(mosquitto ${mosquitto_SOURCE_DIR})
		endif (WIN32)
	else (STATIC_WEBSOCKETS)
		broker_link_libraries(PRIVATE websockets_shared)
	endif (STATIC_WEBSOCKETS)
endif (WITH_WEBSOCKETS)

if (WIN32)
	set_target_properties(mosquitto PROPERTIES ENABLE_EXPORTS 1)
endif (WIN32)

if (UNIX)
	if (APPLE)
		set_target_properties(mosquitto PROPERTIES LINK_FLAGS "-Wl,-exported_symbols_list -Wl,${mosquitto_SOURCE_DIR}/src/linker-macosx.syms")
	else (APPLE)
		set_target_properties(mosquitto PROPERTIES LINK_FLAGS "-Wl,-dynamic-list=${mosquitto_SOURCE_DIR}/src/linker.syms")
	endif (APPLE)
endif (UNIX)

install(TARGETS mosquitto RUNTIME DESTINATION "${CMAKE_INSTALL_SBINDIR}")
install(FILES ../include/mosquitto_broker.h ../include/mosquitto_plugin.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")


# broker_link_libraries(PUBLIC config-header)
# broker_link_libraries(PRIVATE common-options)

if (WITH_THREADING AND NOT WIN32)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)

	broker_link_libraries(PRIVATE Threads::Threads)
endif()

set_target_properties(mosquitto PROPERTIES
	ENABLE_EXPORTS 1
)

install(TARGETS mosquitto
	RUNTIME DESTINATION "${CMAKE_INSTALL_SBINDIR}"
)

if (WITH_EMBEDD_BROKER)
	set_target_properties(libmosquitto_broker PROPERTIES
		OUTPUT_NAME mosquitto_broker
		VERSION ${VERSION}
		SOVERSION 1
		POSITION_INDEPENDENT_CODE 1
	)
	
	if (WITH_STATIC_LIBRARIES)
		set_target_properties(libmosquitto_broker_static PROPERTIES
		OUTPUT_NAME mosquitto_broker_static
		VERSION ${VERSION}
		SOVERSION 1
		POSITION_INDEPENDENT_CODE 1
	)
	endif()
	
	
	if(APPLE)
		set_target_properties(libmosquitto_broker PROPERTIES
			LINK_FLAGS "-exported_symbols_list ${mosquitto_SOURCE_DIR}/src/embedd-broker-macos.syms"
		)
	else (APPLE)
		set_target_properties(libmosquitto_broker PROPERTIES
			LINK_FLAGS "-Wl,--version-script=${mosquitto_SOURCE_DIR}/src/embedd-broker.version"
                )
	endif (APPLE)

	install(FILES
		${mosquitto_SOURCE_DIR}/include/mosquitto/embedded_broker.h
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/mosquitto"
	)
	
	install(TARGETS libmosquitto_broker
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	)
	
	if (WITH_STATIC_LIBRARIES)
		install(TARGETS libmosquitto_broker_static ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")
	endif()

endif()
