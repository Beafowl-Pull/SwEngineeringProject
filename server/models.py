from sqlalchemy import Column, Integer, String, ForeignKey, Text
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship
from pydantic import BaseModel

Base = declarative_base()


# SQLAlchemy User model
class User(Base):
    __tablename__ = "users"
    id = Column(Integer, primary_key=True, index=True)
    username = Column(String(50), unique=True, index=True)
    password = Column(String(128))  # Hashed password

    # Relationships for sent and received messages
    sent_messages = relationship("Message", foreign_keys="[Message.sender_id]", back_populates="sender")
    received_messages = relationship("Message", foreign_keys="[Message.receiver_id]", back_populates="receiver")


# SQLAlchemy Message model
class Message(Base):
    __tablename__ = "messages"
    id = Column(Integer, primary_key=True, index=True)
    content = Column(Text)

    # Foreign keys for sender and receiver
    sender_id = Column(Integer, ForeignKey("users.id"))
    receiver_id = Column(Integer, ForeignKey("users.id"))

    # Relationships
    sender = relationship("User", foreign_keys=[sender_id], back_populates="sent_messages")
    receiver = relationship("User", foreign_keys=[receiver_id], back_populates="received_messages")


# Pydantic model for MessageCreate (used for input validation)
class MessageCreate(BaseModel):
    content: str
    sender_id: int
    receiver_id: int

    class Config:
        from_attributes = True


# Pydantic model for MessageResponse (used for response output)
class MessageResponse(BaseModel):
    id: int
    content: str
    sender_id: int
    receiver_id: int

    class Config:
        from_attributes = True
