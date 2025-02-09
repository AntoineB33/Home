from PyQt5.QtWidgets import QTableView, QAbstractItemView
from PyQt5.QtCore import Qt

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

        # Connect scroll events
        self.verticalScrollBar().valueChanged.connect(self.handle_vertical_scroll)
        self.horizontalScrollBar().valueChanged.connect(self.handle_horizontal_scroll)

        self.prev_vertical_scroll = None
        self.prev_horizontal_scroll = None

    def resizeEvent(self, event):
        """
        Adjust the number of computed rows/columns based on window size.
        """
        super().resizeEvent(event)
        self.adjust_grid_size()

    def adjust_grid_size(self):
        """
        Dynamically adjusts the number of rows and columns based on the current viewport size.
        """
        if not self.model():
            return
        
        row_height = self.rowHeight(0) if self.rowHeight(0) > 0 else 20  # Default row height
        col_width = self.columnWidth(0) if self.columnWidth(0) > 0 else 80  # Default column width

        visible_rows = self.viewport().height() // row_height + 1
        visible_cols = self.viewport().width() // col_width + 1

        self.controller.adjust_rows_count(visible_rows)
        self.controller.adjust_columns_count(visible_cols)

    def handle_vertical_scroll(self, value):
        """
        Adjust the number of rows based on vertical scrolling.
        """
        if value != self.prev_vertical_scroll:
            self.prev_vertical_scroll = value
            self.adjust_grid_size()
    
    def handle_horizontal_scroll(self, value):
        """
        Adjust the number of columns based on horizontal scrolling.
        """
        if value != self.prev_horizontal_scroll:
            self.prev_horizontal_scroll = value
            self.adjust_grid_size()