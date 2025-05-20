from .database_interface import DatabaseInterface

class OracleAdapter(DatabaseInterface):
    def __init__(self, host, database, user, password, port=1521):
        self.host = host
        self.database = database
        self.user = user
        self.password = password
        self.port = port
        print("Aviso: OracleAdapter é um stub e não está implementado ainda")

    def connect(self):
        print("Método connect não implementado para Oracle")
        pass

    def disconnect(self):
        print("Método disconnect não implementado para Oracle")
        pass

    def execute_query(self, query, params=None):
        print("Método execute_query não implementado para Oracle")
        return False

    def fetch_all(self, query, params=None):
        print("Método fetch_all não implementado para Oracle")
        return []

    def fetch_one(self, query, params=None):
        print("Método fetch_one não implementado para Oracle")
        return None

    def insert(self, table, data):
        print("Método insert não implementado para Oracle")
        return None

    def update(self, table, data, condition):
        print("Método update não implementado para Oracle")
        return 0

    def delete(self, table, condition):
        print("Método delete não implementado para Oracle")
        return 0