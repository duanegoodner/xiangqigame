from typing import Callable

from bs4 import BeautifulSoup, element
from pathlib import Path


class TableDescriptionModifier:
    def __init__(
        self,
        html_file_path: Path,
        div_retrieval_function: Callable[[BeautifulSoup], element.Tag],
        modify_function: Callable[[element.Tag, BeautifulSoup], None],
    ):
        self.html_file_path = html_file_path
        with html_file_path.open(mode="r", encoding="utf-8") as f:
            soup = BeautifulSoup(f, "html.parser")
        self.soup = soup
        self.div_retrieval_function = div_retrieval_function
        self.modify_function = modify_function

    def modify_description(self):
        div_to_modify = self.div_retrieval_function(self.soup)
        self.modify_function(div_to_modify, self.soup)
        with self.html_file_path.open(mode="w", encoding="utf-8") as out_file:
            out_file.write(self.soup.prettify())


def get_table_description_div(soup: BeautifulSoup) -> element.Tag:
    content_div = soup.find("div", id="doc-content")
    contents_divs = content_div.find_all("div", class_="textblock")
    if len(contents_divs) != 1:
        error_msg = (
            f"Expected len(contents_divs) to be 1, but found "
            f"{len(contents_divs)}"
        )
        raise ValueError(error_msg)

    return contents_divs[0]


def modify_namespaces_table_description(
    div_to_modify: element.Tag, soup: BeautifulSoup
):
    first_part = (
        "The list below includes all top-level namespaces including Python "
        "package "
    )
    bold_tag = soup.new_tag("strong")
    bold_tag.string = "xiangqipy"
    second_part = "."

    # Create a line break tag
    br_tag = soup.new_tag("br")

    third_part = "Each namespace entry can be expanded to show sub-namespaces and/or classes."

    div_to_modify.clear()

    # Append the new structure to the original tag
    div_to_modify.append(first_part)
    div_to_modify.append(bold_tag)
    div_to_modify.append(second_part)
    div_to_modify.append(br_tag)  # Insert the line break
    div_to_modify.append(third_part)


def modify_class_table_description(div_to_modify: element.Tag, soup: BeautifulSoup):
    first_part = (
        "The list below includes all top-level namespaces including Python "
        "package "
    )
    bold_tag = soup.new_tag("strong")
    bold_tag.string = "xiangqipy"
    second_part = "."

    # Create a line break tag
    br_tag_1 = soup.new_tag("br")

    third_part = "Anonymously namespaced CRTP interface classes are also included."

    br_tag_2 = soup.new_tag("br")

    fourth_part = "Each namespace entry can be expanded to show sub-namespaces and/or classes."

    div_to_modify.clear()

    # Append the new structure to the original tag
    div_to_modify.append(first_part)
    div_to_modify.append(bold_tag)
    div_to_modify.append(second_part)
    div_to_modify.append(br_tag_1)  # Insert the line break
    div_to_modify.append(third_part)
    div_to_modify.append(br_tag_2)
    div_to_modify.append(fourth_part)



if __name__ == "__main__":
    namespace_description_modifier = TableDescriptionModifier(
        html_file_path=Path(__file__).parent.parent
        / "html"
        / "namespaces.html",
        div_retrieval_function=get_table_description_div,
        modify_function=modify_namespaces_table_description,
    )

    namespace_description_modifier.modify_description()

    class_list_description_modifier = TableDescriptionModifier(
        html_file_path=Path(__file__).parent.parent / "html" / "annotated.html",
        div_retrieval_function=get_table_description_div,
        modify_function=modify_class_table_description,
    )
    class_list_description_modifier.modify_description()
