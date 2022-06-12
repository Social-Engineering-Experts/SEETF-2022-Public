from fastapi import FastAPI, HTTPException, Depends
from fastapi.staticfiles import StaticFiles
from sqlalchemy.orm import Session

from db import crud, models, schemas
from db.database import SessionLocal, engine

from flag import flag

models.Base.metadata.create_all(bind=engine)

app = FastAPI()
p = 2**255 - 19
start = 314159265358979323846264338327950288419716939937510582097494459230781640628


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


@app.get("/api/get_start")
async def get_start():
    return {"room": str(start)}


@app.get("/api/get_next")
async def get_next(current: str, move: str):

    if not current.isdigit():
        raise HTTPException(status_code=400, detail="`current` isn't a number!")

    current = int(current)
    if move not in [*"+-/"]:
        raise HTTPException(status_code=400, detail="Invalid move!")

    if move == "+":
        return {"room": str((current + 1) % p), "is_finish": False}
    if move == "-":
        return {"room": str((current - 1) % p), "is_finish": False}

    if current == 0:
        return {"room": "infinity", "is_finish": True}

    return {"room": str(pow(current, -1, mod=p)), "is_finish": False}


@app.post("/api/submit")
async def submit(entry: schemas.EntryCreate, db: Session = Depends(get_db)):

    player_name = entry.player_name
    if len(player_name) > 100:
        raise HTTPException(
            status_code=400, detail="Player name too long! <100 chars pls >--<"
        )
    moves = entry.moves

    if len(moves) > 50000:
        raise HTTPException(status_code=400, detail="Too many moves!")

    s = start
    for idx, m in enumerate(moves):
        if m == "+":
            s = (s + 1) % p
        elif m == "-":
            s = (s - 1) % p
        elif m == "/":
            if s == 0:
                s = "infinity"
                break
            s = pow(s, -1, p)
        else:
            raise HTTPException(status_code=400, detail=f"Invalid move {m}")
    idx += 1

    if s != "infinity":
        raise HTTPException(
            status_code=400, detail=f"Invalid moves. Final room isn't infinity!"
        )

    place = crud.add_entry(
        db,
        schemas.EntryCreate(
            player_name=player_name, moves=moves[:idx]  # Truncate the moves
        ),
    )

    if idx >= 1000:
        desc = f"{idx} moves! You're in the {place} place! Try less than 1000 moves for the flag"
    else:
        desc = f"{idx} moves! You're in the {place} place! Here's the flag! {flag}"

    return {"success": True, "detail": desc}


@app.get("/api/leaderboard")
async def get_leaderboard(db: Session = Depends(get_db)):
    return crud.get_leaderboard(db)


app.mount("/", StaticFiles(directory="static", html=True), name="static")
