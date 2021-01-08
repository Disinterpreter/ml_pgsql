#include "CPGConnection.h"

CPGConnection::CPGConnection(const char* connectionString) {
	m_databaseConnection.reset(PQconnectdb(connectionString), &PQfinish);
	if (PQstatus(m_databaseConnection.get()) != CONNECTION_OK && PQsetnonblocking(m_databaseConnection.get(), 1) != 0) {
		throw std::runtime_error(PQerrorMessage(m_databaseConnection.get()));
	}
}

std::shared_ptr<PGconn> CPGConnection::connection() const {
	return m_databaseConnection;
}