
#se estiver usando o compilador Microsoft:
contains(QMAKE_CC , cl)  {

	message("microsoft")

	DEFINES -= UNICODE
	DEFINES -= _UNICODE
	DEFINES += _CRT_SECURE_NO_WARNINGS
	DEFINES +=  _WIN32_WINDOWS 0x0501 
	DEFINES +=	 WIN32
	DEFINES  +=  _WINDOWS 
	DEFINES += _VC80_UPGRADE=0x0600 
	DEFINES += _MBCS
	DEFINES -=  _AFXDLL
	DEFINES -= _AFX
	
	QMAKE_CXXFLAGS += /GF /FD /EHsc /Gy 
			
	contains ( TEMPLATE, lib ) {
		DEFINES += _USRDLL 
		DEFINES += _WINDLL
	}
}
else {
	message("gcc")
}

windows: MYPROJECTS_DIR = C:/cursoQt
!windows: MYPROJECTS_DIR = /cursoQt

INCLUDE_DIR = $$MYPROJECTS_DIR/include
LIBS_DIR = $$MYPROJECTS_DIR/lib

INCLUDEPATH += $$INCLUDE_DIR

# -L (mai�sculo) indica o(s) diretorio(s) padrao para buscar libs na linkagem
# sejam elas bibliotecas est�ticas (contendo o bin�rio) 
# sejam elas meras "implibs" (apenas enderecos das fun��es numa DLL)

LIBS += -L$$LIBS_DIR

#se est� compilando uma lib (est�tica ou dinamica) for�a que seja gravada em um diretorio padr�o
contains ( TEMPLATE, lib ) {
	DESTDIR = $$LIBS_DIR

	 #se � uma lib, e se est� compilando em modo debug, acrescenta "d" ao nome do "Target"
    build_pass:CONFIG(debug, debug|release) {
		TARGET = $$join(TARGET,,,d)
		}
 }

# #defines �teis (para modos debug e release) de forma a propiciar compila��es condicionais
# -por exempo checagens extras para modo debug: 
#  #ifdef _DEBUG 
#				if (pointer == 0 ) abort();
#  #endif

build_pass:CONFIG(debug, debug|release) {
	 #debug
	 DEFINES += _DEBUG DEBUG
}
else {
	#release
   DEFINES += NDEBUG  _NODEBUG
}

