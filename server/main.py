from typing import List

from fastapi import FastAPI, Depends, HTTPException
from sqlalchemy.orm import Session
from database import SessionLocal, engine
from models import User, Message, MessageResponse, MessageCreate
from schemas import UserCreate, UserResponse, LoginRequest
from security import hash_password, verify_password
from fastapi import HTTPException

app = FastAPI()


# Dependency for getting a DB session
def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


# User registration route
@app.post("/register", response_model=UserResponse)
def register_user(user: UserCreate, db: Session = Depends(get_db)):
    # Check if the user already exists
    if db.query(User).filter(User.username == user.username).first():
        raise HTTPException(status_code=400, detail="Username already taken")

    hashed_pw = hash_password(user.password)
    new_user = User(username=user.username, password=hashed_pw)

    db.add(new_user)
    db.commit()
    db.refresh(new_user)

    return new_user


# Login route
@app.post("/login", response_model=UserResponse)
def login_user(login: LoginRequest, db: Session = Depends(get_db)):
    # Check if the user exists
    user = db.query(User).filter(User.username == login.username).first()

    if not user or not verify_password(login.password, user.password):
        raise HTTPException(status_code=400, detail="Invalid username or password")

    return user


@app.post("/messages/", response_model=MessageResponse)
def send_message(message: MessageCreate, sender_id: int, db: Session = Depends(get_db)):
    sender = db.query(User).filter(User.id == sender_id).first()
    receiver = db.query(User).filter(User.id == message.receiver_id).first()

    if not sender or not receiver:
        raise HTTPException(status_code=404, detail="Sender or receiver not found")

    new_message = Message(content=message.content, sender_id=sender_id, receiver_id=message.receiver_id)
    db.add(new_message)
    db.commit()
    db.refresh(new_message)

    return new_message


# Endpoint to get all messages for a user
@app.get("/messages/{user_id}", response_model=List[MessageResponse])
def get_messages(user_id: int, db: Session = Depends(get_db)):
    messages = db.query(Message).filter(
        (Message.sender_id == user_id) | (Message.receiver_id == user_id)
    ).all()

    if not messages:
        raise HTTPException(status_code=404, detail="No messages found")

    return messages

@app.get("/conversations/{user_id}", response_model=List[MessageResponse])
def get_conversations(user_id: int, db: Session = Depends(get_db)):
    conversations = db.query(Message).filter(
        (Message.sender_id == user_id) | (Message.receiver_id == user_id)
    ).all()

    if not conversations:
        raise HTTPException(status_code=404, detail="No conversations found")

    return conversations

@app.get("/users", response_model=List[UserResponse])
def get_users(db: Session = Depends(get_db)):
    users = db.query(User).all()
    return users

@app.get("/user/{user_id}", response_model=UserResponse)
def get_user(user_id: int, db: Session = Depends(get_db)):
    user = db.query(User).filter(User.id == user_id).first()

    if not user:
        raise HTTPException(status_code=404, detail="User not found")

    return user
