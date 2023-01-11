const express = require('express')
const router = express.Router()

const CoursController = require('../Controllers/CoursController')

router.post('/store', CoursController.store)
router.get ('/', CoursController.showAll)
router.get ('/showByName', CoursController.showByName)
router.put ('/update', CoursController.update)
router.delete('/delete',CoursController.destroy)

module.exports = router