#pragma once

#include <stdexcept>
#include <memory>
#include <libpq-fe.h>

class CPGConnection {
public: 
	CPGConnection(const char* connectionString);
	std::shared_ptr<PGconn> connection() const;
private:
	std::shared_ptr<PGconn> m_databaseConnection;
};