from PyQt5.QtCore import Qt, QAbstractTableModel, QModelIndex, QVariant
from PyQt5.QtGui import QColor

class InfiniteTableModel(QAbstractTableModel):
    """
    A QAbstractTableModel that dynamically expands and contracts based on user scrolling.
    """
    def __init__(self, storage, parent=None):
        super().__init__(parent)
        self._storage = storage
        self._row_count = 20  # Start small (only visible rows)
        self._col_count = 10  # Start small (only visible columns)
        self._column_colors = {}  # Store column background colors

    def rowCount(self, parent=QModelIndex()):
        return self._row_count

    def columnCount(self, parent=QModelIndex()):
        return self._col_count

    def data(self, index, role=Qt.DisplayRole):
        if not index.isValid():
            return QVariant()

        row, col = index.row(), index.column()

        if role == Qt.DisplayRole or role == Qt.EditRole:
            return self._storage.get_value(row, col)

        if role == Qt.BackgroundRole and row == 0:  # Apply color only to first row
            return QColor(self._column_colors.get(col, Qt.white))

        return QVariant()

    def setData(self, index, value, role=Qt.EditRole):
        if index.isValid() and role == Qt.EditRole:
            self._storage.set_value(index.row(), index.column(), value)
            self.dataChanged.emit(index, index, [Qt.DisplayRole, Qt.EditRole])
            return True
        return False

    def set_column_color(self, col, color):
        """Set the background color of the first cell in the given column."""
        self._column_colors[col] = color
        index = self.index(0, col)  # First row, specified column
        self.dataChanged.emit(index, index, [Qt.BackgroundRole])

    def expand_rows(self, new_count):
        """Expand row count if needed."""
        if new_count > self._row_count:
            self.beginResetModel()
            self._row_count = new_count
            self.endResetModel()

    def expand_columns(self, new_count):
        """Expand column count if needed."""
        if new_count > self._col_count:
            self.beginResetModel()
            self._col_count = new_count
            self.endResetModel()

    def shrink_rows(self):
        """Shrink rows if all cells in the extra rows are empty."""
        last_filled_row = max((row for row, col in self._storage._data.keys()), default=0)
        new_count = max(20, last_filled_row + 1)  # Keep at least 20 rows
        if new_count < self._row_count:
            self.beginResetModel()
            self._row_count = new_count
            self.endResetModel()

    def shrink_columns(self):
        """Shrink columns if all cells in the extra columns are empty."""
        last_filled_col = max((col for row, col in self._storage._data.keys()), default=0)
        new_count = max(10, last_filled_col + 1)  # Keep at least 10 columns
        if new_count < self._col_count:
            self.beginResetModel()
            self._col_count = new_count
            self.endResetModel()

    def flags(self, index):
        if not index.isValid():
            return Qt.NoItemFlags
        return Qt.ItemIsEditable | Qt.ItemIsSelectable | Qt.ItemIsEnabled
