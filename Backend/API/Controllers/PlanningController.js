const Planning = require('../../Database/Planning')

//Creer un nouveau plannig
const store = async (req, res, next) => {
    try {
        let planning = Planning({
            jourSemaine : req.body.jourSemaine,
            heureDebut : req.body.heureDebut,
            heureFin : req.body.heureFin,
            cours : req.body.cours
        })
        let response = await planning.save()
        res.status(200).json({
            message : 'Nouveau planning ajouté !', content: await response.populate('cours')
        })

    } catch (error) {
        res.status(400).json({
            message : 'Une erreur est survenue !', content: error.message
        })
    }
}


//Afficher la liste de planning
const showAll=(req, res, next) => {
    Planning.find().sort({jourSemaine: 1, heureDebut: 1}).populate('cours')
    .then(response =>{
        let finalTab = []
        response.forEach((value)=>{
            finalTab.push({_id: value._id, __v: value.__v,
                jourSemaine: value.jourSemaine, heureDebut: value.heureDebut, heureFin: value.heureFin,
                cours: value.cours, hDebut: value.hDebut, hFin: value.hFin})
        })
        res.status(200).json(finalTab)
    })
    .catch(error =>{
        res.status(400).json({
            message:'Une erreur est survenue', content: error.message
        })
    })
}

// Modifier un planning

const update = async (req, res, next) => {
    try {
        let plan = await Planning.findById(req.body.id)
        if(!plan) throw new Error('Planning non trouvé.')

        if(req.body.jourSemaine) plan.jourSemaine = req.body.jourSemaine
        if(req.body.heureDebut) plan.heureDebut = req.body.heureDebut
        if(req.body.heureFin) plan.heureFin = req.body.heureFin
        if(req.body.cours) plan.cours = req.body.cours
        await plan.save()

        res.status(200).json({
            message: 'Planning modifié avec sucèss!'
        })

    } catch (error) {
        res.status(400).json({
            message:'Une erreur est survenue!', content: error.message
        })
    }
}

// Supprimer un planning
const destroy = (req, res, next) => {
    let planningID = req.params.id
    Planning.findByIdAndRemove(planningID)
    .then(() => {
        res.status(200).json({
            message : 'Suppression du planning avec succèss !'
        })
    })
    .catch(error => {
        res.status(400).json({
            message : 'Une erreur est survenue !', content: error.message
        })
    })
}


// exportation des fonctions
module.exports = {
    store, showAll, update, destroy
}
   