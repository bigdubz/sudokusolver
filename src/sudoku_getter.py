from bs4 import BeautifulSoup as Soup
import requests


def get_html() -> Soup:
    url = "https://east.websudoku.com/?level=4"
    response = requests.get(url)
    return Soup(response.text, "html.parser")


def get_array():
    soup = get_html()
    table = soup.find("table", class_="t")
    array = [['.' for _ in range(9)] for _ in range(9)]
    for y, a in enumerate(table):
        for x, b in enumerate(a):
            b = str(b)
            ind = b.find("value")
            if ind > 0:
                array[y][x] = b[ind+len("value=\""): ind + len("value=\"")+1]

    return array


def print_game(array):
    print("\t\tvec![")
    for i in array:
        print(f"\t\t\tvec!{i},")
    print("\t\t];")


if __name__ == '__main__':
    print_game(get_array())
