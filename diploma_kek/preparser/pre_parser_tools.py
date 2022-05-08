from googletrans import Translator


class PreParser:
    SRC_LANGUAGE = 'ru'
    DEST_LANGUAGE = 'en'

    def __init__(self):
        self.translator = Translator()

    def parse_requests(self, text: str = None, filename: str = None) -> list:
        if text is None and filename is None:
            raise self.ArgumentError()

        if filename is not None:
            with open(filename, 'r+') as f:
                data = f.readline()

            return [
                self.translator.translate(line, src=self.SRC_LANGUAGE, dest=self.DEST_LANGUAGE).text
                for line in data if line.strip() != ''
            ]

        return [
            self.translator.translate(text, src=self.SRC_LANGUAGE, dest=self.DEST_LANGUAGE).text
        ]

    class ArgumentError(Exception):
        def __init__(self, message="Incorrect function arguments"):
            self.message = message
            super().__init__(self.message)
