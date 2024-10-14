from bs4 import BeautifulSoup
from pathlib import Path

# Path to the HTML file
file_path = Path(__file__).parent / "test.html"

with file_path.open(mode="r", encoding="utf-8") as file:
    html_content = file.read()

# Parse the HTML
soup = BeautifulSoup(html_content, 'html.parser')

# Find the specific div by ID
div_to_replace = soup.find('div', id='content')

# Check if the div exists and replace its content
if div_to_replace:
    div_to_replace.string = "New content in the div"

# Write the modified HTML back to the file
with open(file_path, 'w', encoding='utf-8') as file:
    file.write(soup.prettify())
