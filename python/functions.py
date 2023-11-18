from json import load, dump

PATH = "/home/simone/Desktop/My projects/Recipes App/python/data.json"


# function to update indexes of a list
def reindex(list_of_dicts: list):
    for i in range(len(list_of_dicts)):
        list_of_dicts[i]["index"] = i


class Data:
    data: dict

    def __init__(self) -> None:
        self.readData()

    # function that will parse the json file
    def readData(self) -> None:
        with open(PATH, "r") as file:
            self.data = load(file)

    # function that will rewrite the data in the json file
    def writeData(self) -> None:
        for key in self.data.keys():
            reindex(self.data[key])

        with open(PATH, "w") as file:
            dump(self.data, file, indent=2)

    # function that will delete dish based on key and index
    def deleteDish(self, key: str, index: int) -> None:
        try:
            self.data[key] = [dish for dish in self.data[key] if dish["index"] != index]
        except KeyError:
            print("The key does not exist in deleteDish")

    # function that will return unique index for new dish based on key
    def getIndex(self, key: str) -> int:
        try:
            return self.data[key][-1]["index"] + 1
        except IndexError and KeyError:
            print("There was an error in getIndex")

    # function that will add a new dish to a specific key
    def addNewDish(self, key: str, dish: dict) -> None:
        try:
            self.data[key].append(dish)
        except KeyError:
            print("The key does not exist in addNewDish")

    # function that will iterate over all keys
    def iterOverKeys(self) -> str:
        for key in self.data.keys():
            yield key

    # function that will iterate over dishes based in key
    def iterOverDishes(self, key: str) -> dict:
        try:
            for dish in self.data[key]:
                yield dish
        except KeyError:
            print("The key does not exist in iterOverDishes")
