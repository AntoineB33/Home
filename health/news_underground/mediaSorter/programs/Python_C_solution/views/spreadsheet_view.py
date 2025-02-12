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

    def resizeEvent(self, event):
        """
        Adjust the number of computed rows/columns based on window size.
        """
        super().resizeEvent(event)
        self.adjust_grid_size()

    def adjust_grid_size(self, additional_rows=0, additional_cols=0):
        """
        Dynamically adjusts the number of rows and columns based on the current viewport size.
        """
        if not self.model():
            return
        
        row_height = self.rowHeight(0) if self.rowHeight(0) > 0 else 20  # Default row height
        col_width = self.columnWidth(0) if self.columnWidth(0) > 0 else 80  # Default column width

        visible_rows = self.viewport().height() // row_height + 2
        visible_cols = self.viewport().width() // col_width + 2

        self.controller.adjust_row_count(visible_rows)
        self.controller.adjust_col_count(visible_cols)
        print(f"self.viewport().height() // row_height : {self.viewport().height() // row_height}")
        print(f"self.viewport().width() % col_width : {self.viewport().width() % col_width}")
    
    def get_last_visible_row(self):
        """Returns the index of the last fully visible row in the table."""
        if not self.model():
            return -1  # No model set
        
        first_visible_row = self.rowAt(0)  # Row at the top of the viewport
        viewport_height = self.viewport().height()
        
        last_visible_row = self.rowAt(viewport_height - 1)  # Row at the bottom
        if last_visible_row == -1:
            # If the last row isn't fully visible, estimate using row height
            row_height = self.rowHeight(first_visible_row) if first_visible_row >= 0 else row_height
            approx_last_row = first_visible_row + (viewport_height // row_height)
            return min(approx_last_row, self.model().rowCount() - 1)
        
        return last_visible_row


    def handle_vertical_scroll(self, value):
        if value == self.verticalScrollBar().maximum():
            self.controller.load_more_rows()
        else:
    
    def handle_horizontal_scroll(self, value):
        """
        Adjust the number of columns based on horizontal scrolling.
        """
        if value != self.prev_horizontal_scroll:
            self.prev_horizontal_scroll = value
            self.adjust_grid_size()