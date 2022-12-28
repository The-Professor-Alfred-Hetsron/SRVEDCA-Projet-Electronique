const Admin = require('../Database/Admin')
const bcrypt = require('./AuthController').bcrypt

// Create an new admin (Create)
const store = (req, res, next) => {
    bcrypt.hash(req.body.password, 10, (err, hashedPass) =>{
        if(err){
            res.json({
                error: err
            })
        }
    })

    let admin = new Admin({
        nom: req.body.name,
        email: req.body.email,
        tel: req.body.phone,
        motdepasse: hashedPass
    })
    admin.save()
    .then(response =>{
        res.json({
            message: 'Administrateur enregistré avec sucèss!'
        })
    })
    .catch(error => {
        res.json({
            message:'Une erreur est survenue!'
        })
    })
}

// Show the list of admin (Read)
const showAll =(req, res, next) => {
    Admin.find()
    .then(response =>{
        res.json({
            response
        })
    })
    .catch(error => {
        res.json({
            message:'Une erreur est survenue!'
        })
    })
}

// Show single admin by name (Read)
const showByName = (req, res, next) =>{
    let adminName = req.body.nom
    Admin.findOne({$where: {nom: adminName}})
    .then(response => {
        res.json({
            response
        })
    })
    .catch(error => {
        res.json({
            message:'Une erreur est survenue!'
        })
    })
}

// Update an admin (Update)
const update = (req, res, next) =>{
    let adminID = req.body.adminID

    let updateData = {
        nom: req.body.name,
        email: req.body.email,
        tel: req.body.tel,
        motdepasse: req.body.password
    }

    Admin.findByIdAndUpdate(adminID, {$set: updateData})
    .then(() => {
        res.json({
            message: 'Administrateur modifié avec sucèss!'
        })
    })
    .catch(error => {
        res.json({
            message:'Une erreur est survenue!'
        })
    })
}

// Delete an admin (Delete)
const destroy = (req, res, next) =>{
    let adminID = req.params.adminID
    Admin.findByIdAndRemove(adminID)
    .then(() => {
        res.json({
            message: 'Administrateur supprimé avec sucèss!'
        })
    })
    .catch(error => {
        res.json({
            message:'Une erreur est survenue!'
        })
    })
}


module.exports = {
    store, showAll, update, destroy
}