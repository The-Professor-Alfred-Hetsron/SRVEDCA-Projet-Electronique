const express = require('express')
const router = express.Router()
const { checkAdmin } = require("../Middleware/AuthMiddlewares")

const { register, login } = require('../Controllers/AuthController')

router.post("/",checkAdmin);
router.post("/register", register);
router.post("/login", login);
module.exports = router