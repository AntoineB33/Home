def find_added_characters(file1_path, file2_path):
    # Read the contents of the first file
    with open(file1_path, 'r') as file1:
        content1 = file1.read()
    
    # Read the contents of the second file
    with open(file2_path, 'r') as file2:
        content2 = file2.read()
    
    # Convert contents to sets to find added characters
    set1 = set(content1)
    set2 = set(content2)
    
    # Find characters in file2 that are not in file1
    added_characters = set2 - set1
    
    return added_characters

# Example usage
file1_path = 'file1.txt'
file2_path = 'file2.txt'
added_chars = find_added_characters(file1_path, file2_path)

print("Added characters:", added_chars)
