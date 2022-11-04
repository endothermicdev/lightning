#! /usr/bin/make
TOOLS := tools/hsmtool tools/check-bolt
TOOLS_SRC := $(TOOLS:=.c)
TOOLS_OBJ := $(TOOLS_SRC:.c=.o)

# Make sure these depend on everything.
ALL_C_SOURCES += $(TOOLS_SRC)
ALL_C_HEADERS +=
ALL_PROGRAMS += $(TOOLS)

TOOLS_COMMON_OBJS = common/utils.o

# python3 -m pip is an alternative that might be used. Separate the bin here.
PIP_EXE=$(shell echo ${PIP} | awk '{print $$1}')
PIP_ARGS := $(shell echo ${PIP} | awk '{print $$2} {print $$3}')

# We force make to relink this every time, to detect version changes.
# Do it atomically, otherwise parallel builds can get upset!
tools/headerversions: $(FORCE) tools/headerversions.o libccan.a
	@trap "rm -f $@.tmp.$$$$" EXIT; $(LINK.o) tools/headerversions.o libccan.a $(LOADLIBES) $(LDLIBS) -o $@.tmp.$$$$ && mv $@.tmp.$$$$ $@

tools/check-bolt: tools/check-bolt.o $(TOOLS_COMMON_OBJS)

tools/hsmtool: tools/hsmtool.o $(TOOLS_COMMON_OBJS) $(BITCOIN_OBJS) common/amount.o common/autodata.o common/bech32.o common/bigsize.o common/configdir.o common/derive_basepoints.o common/descriptor_checksum.o common/hsm_encryption.o common/node_id.o common/type_to_string.o common/version.o wire/fromwire.o wire/towire.o

tools/lightning-hsmtool: tools/hsmtool
	cp $< $@

clean: tools-clean

tools-clean:
	$(RM) tools/headerversions
	$(RM) tools/headerversions.o

tools/pip-install:
	@if which $(PIP_EXE) >> /dev/null; then \
		echo "installing python dependencies"; \
		$(PIP_EXE) $(PIP_ARGS) install -r tools/requirements.txt ; \
	else \
		(echo "Pip not installed, skipping python dependencies") ; \
	fi

include tools/test/Makefile
