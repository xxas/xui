# Compiler
CC = cl.exe

# Compiler Flags
CFLAGS = /std:c++latest /experimental:module /permissive-

# Directories
PARTITIONS_DIR = ../partitions
XUI_DIR = ..

# Files
PARTITIONS = $(PARTITIONS_DIR)/common.functional.cppm \
             $(PARTITIONS_DIR)/common.type_traits.cppm \
             $(PARTITIONS_DIR)/object.composition.cpmp \
             $(PARTITIONS_DIR)/object.linkage.cppm

XUI = $(XUI_DIR)/xui.core.ixx

# Object Files
PARTITIONS_IFC = $(PARTITIONS:.cppm=.ifc)
XUI_IFC = $(XUI:.ixx=.ifc)

# Targets
all: compile_ifc

# Compile Partitions to .ifc
$(PARTITIONS_IFC): %.ifc: %.cppm
	$(CC) $(CFLAGS) /module:interface /module:output $@ $<

# Compile XUI to .ifc
$(XUI_IFC): %.ifc: %.ixx
	$(CC) $(CFLAGS) /module:interface /module:output $@ $<

# Compile all to .ifc
compile_ifc: $(PARTITIONS_IFC) $(XUI_IFC)

# Clean
clean:
	del *.ifc

	# Phony Targets
	.PHONY: all compile_ifc clean