AUTOMAKE_OPTIONS = foreign subdir-objects

SUBDIRS = xstdlib xio xmath xastro xcpp xflash xtime testing xtools

#dist_data_DATA = $(PACKAGE_NAME)
#everything below here creates a module for xlibs in the user's privatemodules directory (if it exists) or in the data directory
# it will create a .lua module for lmod, and a regular module file for env. modules.
# the module will be created in a directory called ${PACKAGE_NAME} and have a filename of ${PACKAGE_VERSION}. This is the format used by modules for versioning.
if NON_DEFAULT_PREFIX
if ENVMODPINST

if PRIVMODINST
MODFILEINSTPATH = `echo ${HOME}"/privatemodules/"${PACKAGE_NAME}`
MODFILEINSTPATHV = `echo ${HOME}"/privatemodules/"${PACKAGE_NAME}"/"${PACKAGE_VERSION}`
MODFILEUSERMESSAGE = "A module was installed in "
MODFILEUSERMESSAGE += ${MODFILEINSTPATH}
MODFILEUSERMESSAGE += "."
else
MODFILEINSTPATH = `echo ${datadir}"/"${PACKAGE_NAME}`
MODFILEINSTPATHV = `echo ${datadir}"/"${PACKAGE_NAME}"/"${PACKAGE_VERSION}`
MODFILEUSERMESSAGE = "The path for module installaction couldn't be found. It was placed in "
MODFILEUSERMESSAGE += ${MODFILEINSTPATH}
MODFILEUSERMESSAGE += "."
endif

install-data-local:
	-$(MKDIR_P) ${MODFILEINSTPATH}
	@echo "#%Module1.0#####################################################################" > ${MODFILEINSTPATHV}
	@echo "##" >>  ${MODFILEINSTPATHV}
	@echo "## "${PACKAGE_NAME}" modulefile" >>  ${MODFILEINSTPATHV}
	@echo "##" >>  ${MODFILEINSTPATHV}
	@echo "## modulefiles/"${PACKAGE_NAME} >>  ${MODFILEINSTPATHV}
	@echo "##" >>  ${MODFILEINSTPATHV}
	@echo "proc ModulesHelp { } {" >>  ${MODFILEINSTPATHV}
	@echo "	global version" >>  ${MODFILEINSTPATHV}
	@echo "" >>  ${MODFILEINSTPATHV}
	@echo "	puts stderr \"\tAdds xlibs to your PATH and PKG_CONFIG_PATH environment variables\"" >>  ${MODFILEINSTPATHV}
	@echo "}" >>  ${MODFILEINSTPATHV}
	@echo "" >>  ${MODFILEINSTPATHV}
	@echo "module-whatis	\"adds xlibs to your PATH and PKG_CONFIG_PATH environment variables\"" >>  ${MODFILEINSTPATHV}
	@echo "" >>  ${MODFILEINSTPATHV}
	@echo "# for Tcl script use only" >>  ${MODFILEINSTPATHV}
	@echo "set	version		"${PACKAGE_VERSION} >>  ${MODFILEINSTPATHV}
	@echo "set	apppath		"${exec_prefix} >>  ${MODFILEINSTPATHV}
	@echo "" >>  ${MODFILEINSTPATHV}
	@echo 'append-path	PATH	$$apppath/bin' >>  ${MODFILEINSTPATHV}
	@echo 'append-path	PKG_CONFIG_PATH $$apppath/lib/pkgconfig' >>  ${MODFILEINSTPATHV}
	@echo ${MODFILEUSERMESSAGE}


else
if LMODINST

if PRIVMODINST
MODFILEINSTPATH = `echo ${HOME}"/privatemodules/"${PACKAGE_NAME}`
MODFILEINSTPATHV = `echo ${MODFILEINSTPATH}"/"${PACKAGE_VERSION}".lua"`
MODFILEUSERMESSAGE = `echo "A module was installed in "${MODFILEINSTPATH}"."`
else
MODFILEINSTPATH = `echo ${datadir}"/"${PACKAGE_NAME}`
MODFILEINSTPATHV = `echo ${MODFILEINSTPATH}"/"${PACKAGE_VERSION}".lua"`
MODFILEUSERMESSAGE = `echo "The path for module installaction couldn't be found. It was placed in "${MODFILEINSTPATH}"."`
endif

install-data-local:
	-$(MKDIR_P) ${MODFILEINSTPATH}
	@echo "-- Create environment variables." > ${MODFILEINSTPATHV}
	@echo "" >> ${MODFILEINSTPATHV}
	@echo "" >> ${MODFILEINSTPATHV}
	@echo "local dir = \"${prefix}\"" >> ${MODFILEINSTPATHV}
	@echo "" >> ${MODFILEINSTPATHV}
	@echo "append-path(	\"PKG_CONFIG_PATH\" pathJoin(dir,\"lib\\pkgconfig\"))" >>  ${MODFILEINSTPATHV}
	@echo "prepend_path( \"PATH\"           ,  pathJoin(dir,\"bin\"))" >> ${MODFILEINSTPATHV}

endif
endif

endif






