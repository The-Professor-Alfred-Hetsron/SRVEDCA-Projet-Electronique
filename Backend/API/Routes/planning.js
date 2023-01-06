const express = require('express')
const router = express.Router()

const PlanningController = require('../Controllers/PlanningController')

router.post('/store', PlanningController.store)
router.get ('/', PlanningController.showAll)
router.post ('/update', PlanningController.update)
router.post('/delete',PlanningController.destroy)

module.exports = router