# controllers/main_controller.py

import sys
from PyQt5.QtWidgets import QApplication
from data.table_storage import TableStorage
from models.infinite_table_model import InfiniteTableModel
from views.spreadsheet_view import SpreadsheetView
from views.floating_button_panel import FloatingButtonPanel

class MainController:
    """
    The Controller ties together the model(s) and view(s) and creates
    a floating button panel that can be dragged around.
    """
    def __init__(self, collection_filename):
        self.app = QApplication(sys.argv)
        self.storage = TableStorage(collection_filename)
        self.model = InfiniteTableModel(self.storage)
        self.view = SpreadsheetView()
        self.view.setModel(self.model)
        self.floating_panel = FloatingButtonPanel(self.view, self.model)
        self._position_floating_panel()

    def _position_floating_panel(self):
        parent = self.view
        vertical_sb = parent.verticalScrollBar()
        vertical_width = vertical_sb.width() if vertical_sb and vertical_sb.isVisible() else 0
        init_x = parent.width() - vertical_width - self.floating_panel.width()
        init_y = 0
        self.floating_panel.move(init_x, init_y)
        self.floating_panel.show()

    def run(self):
        self.view.show()
        return self.app.exec_()
