import random
import string

def generate_password(length=12):
    # Define the character sets to use
    lowercase = string.ascii_lowercase
    uppercase = string.ascii_uppercase
    digits = string.digits
    special_characters = string.punctuation
    
    # Combine all character sets
    all_characters = lowercase + uppercase + digits + special_characters
    
    # Generate a password using random choices from the combined set
    password = ''.join(random.choice(all_characters) for _ in range(length))
    
    return password

# Example usage
password = generate_password(16)
print("Generated Password:", password)
