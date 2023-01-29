package main

const (
	ExUsage       = 64 // Command line usage error
	ExDataErr     = 65 // Data format error
	ExNoInput     = 66 // Cannot open input
	ExNoUser      = 67 // Addressee unknown
	ExNoHost      = 68 // Host name unknown
	ExUnavailable = 69 // Service unavailable
	ExSoftware    = 70 // Internal software error
	ExOSErr       = 71 // System error (e.g., can't fork)
	ExOsFile      = 72 // Critical OS file missing
	ExCantCreat   = 73 // Can't create (user) output file
	ExIOErrR      = 74 // Input/output error
	ExTempFail    = 75 // Temp failure; user is invited to retry
	ExProtocol    = 76 // Remote error in protocol
	ExNoPerm      = 77 // Permission denied
	ExConfig      = 78 // Configuration error
)
