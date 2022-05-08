from preparser.pre_parser_tools import PreParser


def main():
    parser = PreParser()
    base_language_requests = parser.parse_requests(text="Какой средний рост y человека?")

    



if __name__ == '__main__':
    main()
