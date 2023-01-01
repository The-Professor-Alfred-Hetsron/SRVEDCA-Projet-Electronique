const express = require('express')
const router = express.Router()
const { checkAdmin } = require("../Middleware/AuthMiddlewares")

const { signin, signup } = require('../Controllers/AuthController')

router.post("/",checkAdmin);
router.post("/register", signup);
router.post("/login", signin);
module.exports = router