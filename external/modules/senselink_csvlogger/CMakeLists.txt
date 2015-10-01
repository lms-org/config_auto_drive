set(SOURCES
  "src/senselink_csvlogger.cpp"
  "src/interface.cpp"
)

set(HEADERS
	"include/senselink_csvlogger.h"
)

include_directories(include)
add_library(senselink_csvlogger MODULE ${SOURCES} ${HEADERS})
target_link_libraries(senselink_csvlogger PRIVATE lmscore sense_link)
