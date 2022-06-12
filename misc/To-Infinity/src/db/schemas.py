from typing import List, Optional

from pydantic import BaseModel


class EntryBase(BaseModel):
    player_name: str


class EntryCreate(EntryBase):
    moves: str


class EntryQueryResponse(EntryBase):
    score: int
