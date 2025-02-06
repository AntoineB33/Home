import requests

# Function to get product data from Open Food Facts API
def get_product_data():
    url = "https://world.openfoodfacts.org/api/v0/product/737628064502.json"  # Example product URL from Open Food Facts
    response = requests.get(url)
    
    if response.status_code == 200:
        data = response.json()
        product_name = data['product']['product_name']
        qr_code = data['product']['code']  # Assuming 'code' refers to the barcode
        packaging = data['product'].get('packaging', 'unknown')  # Get packaging material
        bin_to_throw = categorize_bin(packaging)  # Determine the bin
        return {qr_code: (product_name, bin_to_throw)}
    else:
        print("Failed to retrieve data")
        return {}

# Function to categorize the packaging material into the correct bin
def categorize_bin(packaging):
    packaging_lower = packaging.lower()
    if "plastic" in packaging_lower:
        return "Plastic Recycling Bin"
    elif "glass" in packaging_lower:
        return "Glass Recycling Bin"
    elif "paper" in packaging_lower or "cardboard" in packaging_lower:
        return "Paper/Cardboard Recycling Bin"
    elif "metal" in packaging_lower:
        return "Metal Recycling Bin"
    else:
        return "General Waste Bin"

# Fetch product data
product_qr_map = get_product_data()

# Print the result (QR code, product name, and the bin for disposal)
for qr_code, (product_name, bin_to_throw) in product_qr_map.items():
    print(f"Product: {product_name}, QR Code: {qr_code}, Bin: {bin_to_throw}")
