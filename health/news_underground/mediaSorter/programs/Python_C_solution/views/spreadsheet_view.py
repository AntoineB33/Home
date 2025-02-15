from PyQt5.QtWidgets import QTableView, QAbstractItemView, QHeaderView, QVBoxLayout, QWidget
from PyQt5.QtCore import Qt

class SpreadsheetView(QWidget):
    """
    A QTableView with a frozen first row to act as a static header.
    """
    def __init__(self, controller, parent=None):
        super().__init__(parent)
        self.controller = controller
        
        # Create main table view
        self.table_view = QTableView()
        self.table_view.setModel(controller.get_model())
        self.table_view.setSelectionBehavior(QAbstractItemView.SelectItems)
        self.table_view.setSelectionMode(QAbstractItemView.SingleSelection)
        
        # Create frozen row view
        self.frozen_row_view = QTableView(self)
        self.frozen_row_view.setModel(controller.get_model())  # Use same model
        self.frozen_row_view.verticalHeader().hide()  # Hide row numbers
        self.frozen_row_view.horizontalHeader().setSectionResizeMode(QHeaderView.Fixed)
        self.frozen_row_view.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)  # No separate scrollbar
        self.frozen_row_view.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)  # No scrolling
        self.frozen_row_view.setFixedHeight(self.table_view.rowHeight(0) + 2)  # Match first row height
        
        # Ensure only the first row is visible in frozen view
        self.frozen_row_view.setVerticalScrollMode(QAbstractItemView.ScrollPerPixel)
        self.frozen_row_view.setSelectionMode(QAbstractItemView.NoSelection)
        self.frozen_row_view.setFocusPolicy(Qt.NoFocus)
        
        # Hide all but first row
        self.frozen_row_view.setRowHidden(1, True)
        
        # Synchronize horizontal scrollbars
        self.table_view.horizontalScrollBar().valueChanged.connect(
            self.frozen_row_view.horizontalScrollBar().setValue
        )
        
        # Layout
        layout = QVBoxLayout(self)
        layout.setSpacing(0)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.addWidget(self.frozen_row_view)
        layout.addWidget(self.table_view)
        self.setLayout(layout)

    def resizeEvent(self, event):
        super().resizeEvent(event)
        self.adjust_row_count(self.verticalScrollBar().value())
        self.adjust_col_count(self.horizontalScrollBar().value())

    def handle_vertical_scroll(self, value):
        self.adjust_row_count(value)

    def handle_horizontal_scroll(self, value):
        self.adjust_col_count(value)
    
    def adjust_row_count(self, value):
        if value == self.verticalScrollBar().maximum():
            self.controller.load_more_rows()
        else:
            lastRow = self.rowAt(self.viewport().height() - 1)
            self.controller.load_less_rows(lastRow)
    
    def adjust_col_count(self, value):
        if value == self.horizontalScrollBar().maximum():
            self.controller.load_more_cols()
        else:
            lastCol = self.columnAt(self.viewport().width() - 1)
            self.controller.load_less_cols(lastCol)
    