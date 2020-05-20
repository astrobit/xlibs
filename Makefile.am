AUTOMAKE_OPTIONS = foreign subdir-objects

SUBDIRS = xstdlib xio xmath xastro xcpp xflash xtime testing xtools

#dist_data_DATA = $(PACKAGE_NAME)
#everything below here creates a module for xlibs in the user's privatemodules directory (if it exists) or in the data directory
# it will create a .lua module for lmod, and a regular module file for env. modules.
# the module will be created in a directory called ${PACKAGE_NAME} and have a filename of ${PACKAGE_VERSION}. This is the format used by modules for versioning.
#if INSTALLMOD
#if HAVEENVMOD
#modfile_DATA = ${PACKAGE_VERSION}
#else
#modfile_DATA = ${PACKAGE_VERSION}.lua
#endif
#endif

#MODFILEINSTPATH=`echo ${modfiledir}"/"${PACKAGE_NAME}`
#
#install-data-local:
#	-$(MKDIR_P) ${modfiledir}
#	@echo "#%Module1.0#####################################################################" > ${MODFILEINSTPATH}
#	@echo "##" >>  ${MODFILEINSTPATH}
#	@echo "## "${PACKAGE_NAME}" modulefile" >>  ${MODFILEINSTPATH}
#	@echo "##" >>  ${MODFILEINSTPATH}
#	@echo "## modulefiles/"${PACKAGE_NAME} >>  ${MODFILEINSTPATH}
#	@echo "##" >>  ${MODFILEINSTPATH}
#	@echo "proc ModulesHelp { } {" >>  ${MODFILEINSTPATH}
#	@echo "	global version" >>  ${MODFILEINSTPATH}
#	@echo "" >>  ${MODFILEINSTPATH}
#	@echo "	puts stderr \"\tAdds "${PACKAGE_NAME}" to your PATH and PKG_CONFIG_PATH environment variables\"" >>  ${MODFILEINSTPATH}
#	@echo "}" >>  ${MODFILEINSTPATH}
#	@echo "" >>  ${MODFILEINSTPATH}
#	@echo "module-whatis	\"adds "${PACKAGE_NAME}" to your PATH and PKG_CONFIG_PATH environment variables\"" >>  ${MODFILEINSTPATH}
#	@echo "" >>  ${MODFILEINSTPATH}
#	@echo "# for Tcl script use only" >>  ${MODFILEINSTPATH}
#	@echo "set	version		"${PACKAGE_VERSION} >>  ${MODFILEINSTPATH}
#	@echo "set	apppath		"${exec_prefix} >>  ${MODFILEINSTPATH}
#	@echo "" >>  ${MODFILEINSTPATH}
#	@echo 'append-path	PATH	$$apppath/bin' >>  ${MODFILEINSTPATH}
#	@echo 'append-path	PKG_CONFIG_PATH $$apppath/lib/pkgconfig' >>  ${MODFILEINSTPATH}
#
#
#else #LMOD
#
#MODFILEINSTPATH=`echo ${modfiledir}"/"${PACKAGE_NAME}".lua"`
#
#install-data-local:
#	-$(MKDIR_P) ${modfiledir}
#	@echo "-- Create environment variables." > ${MODFILEINSTPATH}
#	@echo "" >> ${MODFILEINSTPATH}
#	@echo "" >> ${MODFILEINSTPATH}
#	@echo "local dir = \"${prefix}\"" >> ${MODFILEINSTPATH}
#	@echo "" >> ${MODFILEINSTPATH}
#	@echo "append-path(	\"PKG_CONFIG_PATH\" pathJoin(dir,\"lib\\pkgconfig\"))" >>  ${MODFILEINSTPATH}
#	@echo "prepend_path( \"PATH\"           ,  pathJoin(dir,\"bin\"))" >> ${MODFILEINSTPATH}
#
#endif
#endif






