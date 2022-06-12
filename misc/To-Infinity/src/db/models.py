from sqlalchemy import Column, Integer, String
from sqlalchemy.orm import relationship

from .database import Base


class Entry(Base):

    __tablename__ = "leaderboard"

    id = Column(Integer, primary_key=True, index=True)
    player_name = Column(String)
    score = Column(Integer)
    hashed_moves = Column(String)
    salt = Column(String)