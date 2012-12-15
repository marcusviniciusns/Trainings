#ifndef TCPDATAPROTOCOL_H
#define TCPDATAPROTOCOL_H

#include <QtGlobal>  // defini��es gerais, como � o caso dos tipos primitivos padronizados em Qt

// Defini��es comuns para o server e para o cliente.

// Neste exemplo, precisamos apenas de um protocolo para a camada de aplica��o da su�te de protocolos TCP/IP.
// Este protocolo de aplica��o indica quais solicita��es(e dados) o server espera receber de um client.

namespace TcpDataProtocol
{
	typedef quint16 requestType;
	typedef quint32 requestSizeType;

	const requestType showMessage = 1;  // o server deve exibir uma mensagem (tendo como argumento um texto)
	const requestType startProcess = 2;    // o server deve iniciar um processo (tendo como argumento um nome de bin�rio execut�vel)


}



#endif // TCPDATAPROTOCOL_H
