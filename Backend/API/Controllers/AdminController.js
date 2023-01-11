const Admin = require('../../Database/Admin')
const bcrypt = require('bcrypt')

// Create an new admin (Create)
const store = async (req, res, next) => {
    try {
        const hashedPass = await bcrypt.hash(req.body.motdepasse, 12);

        let admin = new Admin({
            nom: req.body.nom,
            email: req.body.email,
            tel: req.body.tel,
            motdepasse: hashedPass,
            role: req.body.role,
        })

        let result = await admin.save()
        res.status(200).json({
            message: 'Administrateur enregistré avec sucèss!'
        })
    } catch (error) {
        res.status(400).json({
            message:'Une erreur est survenue!', content: error.message
        })
    }

}

// Show the list of admin (Read)
const showAll =(req, res, next) => {
    Admin.find()
    .then(response =>{
        res.status(200).json(response)
    })
    .catch(error => {
        res.status(400).json({
            message:'Une erreur est survenue!', content: error.message
        })
    })
}

// Show single admin by Id (Read)
const showById = (req, res, next) =>{
    let id = req.params.id
    Admin.findById(id)
    .then(response => {
        res.status(200).json({
            response
        })
    })
    .catch(error => {
        res.status(400).json({
            message:'Une erreur est survenue!', content: error.message
        })
    })
}

// Update an admin (Update)
const update = async (req, res, next) =>{
    try {
        let adminID = req.body.id
        let result = await Admin.findById(adminID)
        if(!result) throw new Error('Administrateur non trouvé. Id incorrect.')

            result.nom = req.body.nom ? req.body.nom : result.nom,
            result.email = req.body.email ? req.body.email : result.email,
            result.tel = req.body.tel ? req.body.tel : result.tel,
            result.motdepasse = req.body.motdepasse ? await bcrypt.hash(req.body.motdepasse, 12) : result.motdepasse,
            result.role = req.body.role ? req.body.role : result.role

        await result.save()

        res.status(200).json({
            message: 'Administrateur modifié avec sucèss!'
        })
    } catch (error) {
        res.status(400).json({
            message:'Une erreur est survenue!', content: error.message
        })
    }
}

// Delete an admin (Delete)
const destroy = (req, res, next) =>{
    let adminID = req.body.id
    Admin.findByIdAndRemove(adminID)
    .then(() => {
        res.status(200).json({
            message: 'Administrateur supprimé avec sucèss!'
        })
    })
    .catch(error => {
        res.status(400).json({
            message:'Une erreur est survenue!', content: error.message
        })
    })
}


module.exports = {
    store, showAll, showById, update, destroy
}