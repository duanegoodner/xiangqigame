
from bs4 import BeautifulSoup
from pathlib import Path


# Path to the HTML file
file_path = Path(__file__).parent.parent / "html" / "namespaces.html"

# Read the HTML file
with file_path.open(mode="r", encoding = "utf-8") as file:
    html_content = file.read()

# Parse the HTML
soup = BeautifulSoup(html_content, 'html.parser')

# Find the specific div by its class
div_to_replace = soup.find('div', class_='textblock')

# Check if the div exists and replace its content
if div_to_replace:
    div_to_replace.string = "This is the modified content"

# Write the modified HTML back to the file
with file_path.open(mode="w", encoding="utf-8") as file:
    file.write(str(soup))

