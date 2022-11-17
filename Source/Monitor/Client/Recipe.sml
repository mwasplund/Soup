Name: "Monitor.Client"
Description: "The Detours dynamic library that is injected into a child process to intercept system calls."
Language: "C++|0.1"
Version: "1.0.0"
Type: "DynamicLibrary"
Defines: [
	# "ENABLE_MONITOR_DEBUG"
	# "TRACE_DETOUR_CLIENT"
]
Source: [
	"Main.cpp"
]

Dependencies: {
	Runtime: [
		"Detours@4.0.9"
		"../Shared/"
		"Opal@0.5.0"
	]
}
