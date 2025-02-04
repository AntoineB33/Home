from PyQt5.QtCore import (
    Qt,
    QAbstractTableModel,
    QModelIndex,
    QVariant
)

class InfiniteTableModel(QAbstractTableModel):
    """
    A QAbstractTableModel that pretends to have an extremely large (or 'infinite')
    number of rows and columns, backed by a TableStorage object.
    """
    def __init__(self, storage, rows=1000000, columns=1000000, parent=None):
        super().__init__(parent)
        self._row_count = rows
        self._col_count = columns
        self._storage = storage  # our data storage instance

    def rowCount(self, parent=QModelIndex()):
        # Return a large number to simulate many rows
        return self._row_count

    def columnCount(self, parent=QModelIndex()):
        # Return a large number to simulate many columns
        return self._col_count

    def data(self, index, role=Qt.DisplayRole):
        # Handle DisplayRole and EditRole
        if not index.isValid() or role not in (Qt.DisplayRole, Qt.EditRole):
            return QVariant()

        row = index.row()
        col = index.column()

        # Fetch the cell value from the storage object
        return self._storage.get_value(row, col)

    def setData(self, index, value, role=Qt.EditRole):
        # Allow editing the cell value
        if index.isValid() and role == Qt.EditRole:
            row = index.row()
            col = index.column()
            self._storage.set_value(row, col, value)
            # Notify listeners that data changed
            self.dataChanged.emit(index, index, [Qt.DisplayRole, Qt.EditRole])
            return True
        return False

    def flags(self, index):
        # Make cells editable, selectable, enabled
        if not index.isValid():
            return Qt.NoItemFlags
        return Qt.ItemIsEditable | Qt.ItemIsSelectable | Qt.ItemIsEnabled
