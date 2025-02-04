import sys
from PyQt5.QtWidgets import QApplication

from data.table_storage import TableStorage
from models.infinite_table_model import InfiniteTableModel
from views.spreadsheet_view import SpreadsheetView

class MainController:
    """
    The 'Controller' in MVC, tying together the model(s) and view(s).
    """
    def __init__(self):
        # Create the application instance
        self.app = QApplication(sys.argv)

        # Create our data storage
        self.storage = TableStorage()

        # Create the model, passing in our storage
        self.model = InfiniteTableModel(self.storage)

        # Create the view (the table widget)
        self.view = SpreadsheetView()
        self.view.setModel(self.model)

    def run(self):
        """
        Show the view and start the Qt event loop.
        """
        self.view.show()
        return self.app.exec_()
