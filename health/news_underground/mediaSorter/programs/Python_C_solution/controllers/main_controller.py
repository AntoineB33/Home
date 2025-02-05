import sys
from PyQt5.QtWidgets import QApplication
from data.table_storage import TableStorage
from models.infinite_table_model import InfiniteTableModel
from views.spreadsheet_view import SpreadsheetView
from views.floating_button_panel import FloatingButtonPanel

class MainController:
    """
    The Controller ties together the model(s) and view(s) and now also creates
    a floating button panel that can be dragged around.
    """
    def __init__(self):
        self.app = QApplication(sys.argv)
        # Data storage and model
        self.storage = TableStorage()
        self.model = InfiniteTableModel(self.storage)
        # Spreadsheet view
        self.view = SpreadsheetView()
        self.view.setModel(self.model)

        # Create the floating button panel as a child of the view.
        # This ensures its coordinates are relative to the QTableView.
        self.floating_panel = FloatingButtonPanel(self.view)
        self._position_floating_panel()

    def _position_floating_panel(self):
        """
        Initially positions the floating panel at the top right of the view
        (but not covering the scroll bars).
        """
        parent = self.view
        # Get vertical scroll bar to compute the allowed right area.
        vertical_sb = parent.verticalScrollBar()
        vertical_width = vertical_sb.width() if vertical_sb and vertical_sb.isVisible() else 0
        # Place the panel at (x, y) where x is parent's width minus the panel's width
        # and minus the vertical scroll bar's width; y is 0.
        init_x = parent.width() - vertical_width - self.floating_panel.width()
        init_y = 0
        self.floating_panel.move(init_x, init_y)
        self.floating_panel.show()

    def run(self):
        self.view.show()
        return self.app.exec_()
