from PyQt5.QtWidgets import QTableView, QAbstractItemView
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QHeaderView

class SpreadsheetView(QTableView):
    """
    A QTableView that dynamically adjusts row and column counts based on scrolling and window resizing.
    """
    def __init__(self, controller, parent=None):
        super().__init__(parent)
        self.controller = controller  # Use controller instead of calling the model directly
        self.setModel(controller.get_model())  # Set the model via the controller
        self.setSelectionBehavior(QAbstractItemView.SelectItems)
        self.setSelectionMode(QAbstractItemView.SingleSelection)

        # Initialize the frozen row view
        self.init_frozen_row()

        # Connect scroll events
        self.verticalScrollBar().valueChanged.connect(self.handle_vertical_scroll)
        self.horizontalScrollBar().valueChanged.connect(self.handle_horizontal_scroll)

    def init_frozen_row(self):
        # Create a frozen row view
        self.frozen_row_view = QTableView(self)
        self.frozen_row_view.setModel(self.model())
        self.frozen_row_view.setFocusPolicy(Qt.NoFocus)
        self.frozen_row_view.verticalHeader().hide()
        self.frozen_row_view.horizontalHeader().hide()
        self.frozen_row_view.setStyleSheet("QTableView { border: none; }")
        self.frozen_row_view.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.frozen_row_view.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        
        # Synchronize horizontal scrolling with main view
        self.horizontalScrollBar().valueChanged.connect(
            self.frozen_row_view.horizontalScrollBar().setValue
        )
        
        # Synchronize column widths
        self.horizontalHeader().sectionResized.connect(self.update_frozen_column_width)
        
        # Make frozen view ignore mouse events
        self.frozen_row_view.setAttribute(Qt.WA_TransparentForMouseEvents)
        
        # Initial geometry update
        self.update_frozen_view_geometry()
        
        # Update when row 0 is resized
        self.verticalHeader().sectionResized.connect(self.on_row_resized)

        # Ensure column widths match initially
        for col in range(self.model().columnCount()):
            self.frozen_row_view.setColumnWidth(col, self.columnWidth(col))

    def update_frozen_view_geometry(self):
        if self.model().rowCount() == 0:
            self.frozen_row_view.hide()
            return
        # Position below horizontal header and match viewport width
        hheader_height = self.horizontalHeader().height()
        self.frozen_row_view.move(self.frameWidth(), hheader_height)  # Ensure left alignment
        self.frozen_row_view.setFixedWidth(self.viewport().width())
        # Set height to row 0's height
        row0_height = self.rowHeight(0)
        self.frozen_row_view.setFixedHeight(row0_height)
        self.frozen_row_view.show()

    def resizeEvent(self, event):
        super().resizeEvent(event)
        self.adjust_row_count(self.verticalScrollBar().value())
        self.adjust_col_count(self.horizontalScrollBar().value())

        # Ensure column widths match
        for col in range(self.model().columnCount()):
            self.frozen_row_view.setColumnWidth(col, self.columnWidth(col))

    def on_row_resized(self, row, old_height, new_height):
        if row == 0:
            self.frozen_row_view.setFixedHeight(new_height)
            self.update_frozen_view_geometry()

    def update_frozen_column_width(self, logical_index, old_size, new_size):
        self.frozen_row_view.horizontalHeader().resizeSection(logical_index, new_size)

    def handle_vertical_scroll(self, value):
        self.adjust_row_count(value)

    def handle_horizontal_scroll(self, value):
        self.frozen_row_view.horizontalScrollBar().setValue(value)
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
    