from PyQt5.QtWidgets import QTableView

class SpreadsheetView(QTableView):
    """
    A simple subclass of QTableView to represent our spreadsheet-like view.
    """
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Excel-Like Infinite Grid")
        self.resize(800, 600)
