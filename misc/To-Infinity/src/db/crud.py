from sqlalchemy.orm import Session

from . import models, schemas

from hashlib import pbkdf2_hmac
import os


def get_leaderboard(db: Session):
    entries = db.query(models.Entry).all()
    entries = sorted(entries, key=lambda x: (x.score << 64) + x.id)
    entries = [*map(
        lambda e: schemas.EntryQueryResponse(
            player_name = e.player_name,
            score = e.score
        ), entries)]
    return entries


def get_placing(db: Session, score: int) -> int:

    leaderboard = get_leaderboard(db)
    scores = sorted([e.score for e in leaderboard])
    placing = len(scores) - scores[::-1].index(score)

    return placing


def add_entry(db: Session, entry: schemas.EntryCreate) -> int:

    score = len(entry.moves)
    salt = os.urandom(32)
    db_entry = models.Entry(
        player_name = entry.player_name, 
        score = score,
        salt = salt,
        hashed_moves = pbkdf2_hmac('sha256', entry.moves.encode(), salt, 100000)
    )
    db.add(db_entry)
    db.commit()
    db.refresh(db_entry)

    return get_placing(db, score)
