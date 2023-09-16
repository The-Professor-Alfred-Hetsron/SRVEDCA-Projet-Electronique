const Classe = require('../../Database/Classe')
const Presence = require('../../Database/Presence')
const Etudiant = require('../../Database/Etudiant')
const moment = require('moment')

//Obtenir des présences
const getPresences = async (req, res, next) => {
    try {
        const presences = await Presence.find().sort({dateheure: -1}).populate('etudiant').exec()
        let presencesTab = []
        let classeId = req.body.classeId, coursId = req.body.coursId,
        dateDebut = req.body.dateDebut, dateFin = req.body.dateFin //intervalle des dates

        let a_classe = await Classe.findById(classeId)
        if(!a_classe){ //Si la classe n'existe pas
            throw new Error('La classe n\'existe pas. Id incorrect')
        }

        for(const presence of presences){

            if(!presence.etudiant.classe.equals(classeId)){
                continue
            }

            let a_presence = {
                //etudiant: await presence.etudiant.populate('classe'),
                _id: presence._id,
                etudiant: presence.etudiant,
                cours: await presence.getCours(), //L'objet cours correspondant
                date: presence.date,
                heure: presence.heure,
                dateheure: presence.dateheure
            }

            if(!coursId || a_presence.cours._id.equals(coursId)) presencesTab.push(a_presence)
            
        }


        if(dateDebut){dateDebut = moment(dateDebut).unix()
            presencesTab = presencesTab.filter((presence) => {
            return dateDebut <= moment(presence.dateheure).unix()
            })
        }

        if(dateFin){dateFin = moment(dateFin).unix()
            presencesTab = presencesTab.filter((presence) => {
                return dateFin + 86400 > moment(presence.dateheure).unix()
            })
        }

        res.status(200).json(presencesTab)

    } catch (error) {
        res.status(400).json({message: error.message})
    }

}

//Enregistrer une présence
const savePresence = (req, res, next) => { 
    let presence = new Presence({
        etudiant: req.body.etudiantId,
        dateheure: req.body.dateheure
    })

    presence.save().then(() => res.status(200).json({message: 'Présence enregistrée avec succès.'}))
    .catch(error => res.status(400).json({message: error.message}))
}

//Enregistrer une présence sur la base du matricule de l'étudiant
const savePresenceByMatricule = async (req, res, next) => {
    try {
        const {matricule, dateheure} = req.body
        const etudiant = await Etudiant.findOne().where({matricule: matricule})
        if(!etudiant) throw new Error("L'étudiant de matricule " + matricule + " n'existe pas.")

        let presence = new Presence({
            etudiant: etudiant._id,
            dateheure: dateheure
        })

        let cours = await presence.getCours() //On obtient le cours qui se déroule au moment de cette présence

        if(!cours) throw new Error(`L'étudiant ${matricule} (${etudiant.nom}) n'a pas cours en ce moment.`)

        await presence.save()
        res.status(200).json({message: 'Présence enregistrée avec succès.'})
    } catch (error) {
        res.status(400).json({message: error.message})
    }
}

//Enregistrer plusieurs présences
const saveManyPresences = (req, res, next) => {
    //Ici l'attribut presences de req.body est un array contenant des objets Presence
    // Presence.create(JSON.parse(req.body.presences))
    Presence.create(req.body.presences)
    .then(() => res.status(200).json({message: 'Présences enregistrées avec succès.'}))
    .catch(error => res.status(400).json({message: error.message}))
}

//Supprimer une présence
const deletePresence = (req, res, next) => { 

    Presence.findOneAndRemove({_id: req.body.presenceId})
    .then(() => res.status(200).json({message: 'Présence supprimée avec succès.'}))
    .catch(error => res.status(400).json({message: error.message}))
}

module.exports = {getPresences, savePresence, savePresenceByMatricule, saveManyPresences, deletePresence}