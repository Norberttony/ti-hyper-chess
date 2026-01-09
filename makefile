# ----------------------------
# Makefile Options
# ----------------------------

NAME = HYPERCHESS
ICON = icon.png
DESCRIPTION = "Hyper Chess analysis"
COMPRESSED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
