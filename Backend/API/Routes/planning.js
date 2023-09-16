const express = require('express')
const router = express.Router()

const PlanningController = require('../Controllers/PlanningController')
const {checkAdmin} = require('../Middleware/AuthMiddlewares')

router.use(checkAdmin)

router.post('/store', PlanningController.store)
router.get ('/', PlanningController.showAll)
router.put ('/update', PlanningController.update)
router.delete('/delete/:id',PlanningController.destroy)

module.exports = router