from PyQt5.QtWidgets import QTableView, QAbstractItemView
from PyQt5.QtCore import Qt

class SpreadsheetView(QTableView):
    """
    A QTableView that expands and shrinks dynamically as you scroll.
    """
    def __init__(self, model, parent=None):
        super().__init__(parent)
        self.setModel(model)
        self.setSelectionBehavior(QAbstractItemView.SelectItems)
        self.setSelectionMode(QAbstractItemView.SingleSelection)

        # Connect scrollbar signals
        self.verticalScrollBar().valueChanged.connect(self.handle_vertical_scroll)
        self.horizontalScrollBar().valueChanged.connect(self.handle_horizontal_scroll)

    def handle_vertical_scroll(self, value):
        """Adjust row count dynamically when scrolling down or up."""
        max_scroll = self.verticalScrollBar().maximum()
        if value == max_scroll:  # User scrolled to the bottom
            self.model().expand_rows(self.model().rowCount() + 20)  # Add 20 more rows
        elif value == 0:  # User scrolled to the top
            self.model().shrink_rows()  # Shrink rows if empty

    def handle_horizontal_scroll(self, value):
        """Adjust column count dynamically when scrolling right or left."""
        max_scroll = self.horizontalScrollBar().maximum()
        if value == max_scroll:  # User scrolled to the right
            self.model().expand_columns(self.model().columnCount() + 10)  # Add 10 more columns
        elif value == 0:  # User scrolled to the left
            self.model().shrink_columns()  # Shrink columns if empty
