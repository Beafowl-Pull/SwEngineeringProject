from pydantic import BaseModel

# For user registration and login
class UserCreate(BaseModel):
    username: str
    password: str

# For user authentication response
class UserResponse(BaseModel):
    id: int
    username: str

class LoginRequest(BaseModel):
    username: str
    password: str
