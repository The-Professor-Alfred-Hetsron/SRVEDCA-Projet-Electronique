const express = require('express')
const router = express.Router()

const CoursController = require('../Controllers/CoursController')

router.post('/store', CoursController.store)
router.get ('/all', CoursController.showAll)
router.get ('/:nom', CoursController.showByName)
router.put ('/update', CoursController.update)
router.delete('/delete/:id',CoursController.destroy)

module.exports = router