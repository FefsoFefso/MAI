from sqlalchemy import *
from sqlalchemy.engine import create_engine

ENGINE = create_engine('sqlite://')

class Example:
    def __init__(self):
        pass

    @staticmethod
    def get_table():
        table = Table('user', MetaData(ENGINE),
                      Column('user_id', Integer, primary_key=True),
                      Column('user_name', String(16), nullable=False),
                      Column('email_address', String(60)),
                      Column('nickname', String(50), nullable=False)
                      )

        table.create()

        ENGINE.execute(table.insert().values(user_id=1, user_name="perviy user", email_address="kek@mail.ru", nickname="nickkk"))
        ENGINE.execute(table.insert().values(user_id=2, user_name="vtoriy user", email_address="kek228@mail.ru", nickname="dddd"))
