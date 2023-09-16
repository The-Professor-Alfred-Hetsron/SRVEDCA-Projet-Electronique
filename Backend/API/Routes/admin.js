const express = require('express')
const router = express.Router()

const AdminController = require('../Controllers/AdminController')
const {checkAdmin} = require('../Middleware/AuthMiddlewares')

router.use(checkAdmin)

router.post('/store', AdminController.store)
router.get('/all', AdminController.showAll)
router.get('/:id', AdminController.showById)
router.put('/update', AdminController.update)
router.delete('/destroy/:id', AdminController.destroy)

module.exports = router